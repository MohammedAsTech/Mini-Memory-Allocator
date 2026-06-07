#!/usr/bin/env bash
set -u

# Mini Memory Allocator - 300 input tests + compile + sanitizer/leak check
# Run from your project root:
#   chmod +x test_allocator_300.sh
#   ./test_allocator_300.sh

ROOT="$(pwd)"
TEST_DIR="$ROOT/tests_allocator_300_results"
mkdir -p "$TEST_DIR/cases" "$TEST_DIR/logs"

APP="$TEST_DIR/allocator_app"
ASAN_APP="$TEST_DIR/allocator_app_asan"
SUMMARY="$TEST_DIR/summary.txt"
DETAILS="$TEST_DIR/details.txt"
COMPILE_LOG="$TEST_DIR/logs/compile.log"
ASAN_COMPILE_LOG="$TEST_DIR/logs/compile_asan.log"

rm -f "$SUMMARY" "$DETAILS" "$COMPILE_LOG" "$ASAN_COMPILE_LOG"

# Auto-detect common project layouts.
if compgen -G "src/*.cpp" > /dev/null; then
    SOURCES=(src/*.cpp)
else
    SOURCES=(*.cpp)
fi

if [[ -d include ]]; then
    INCLUDES=(-Iinclude -I.)
else
    INCLUDES=(-I.)
fi

# Do not accidentally compile generated tests if this script is run from /mnt/data.
FILTERED_SOURCES=()
for s in "${SOURCES[@]}"; do
    case "$s" in
        *stress_invariants.cpp|*test_main.cpp) ;;
        *) FILTERED_SOURCES+=("$s") ;;
    esac
done
SOURCES=("${FILTERED_SOURCES[@]}")

echo "[BUILD] Compiling normal executable..."
if ! g++ -std=c++20 -Wall -Wextra -pedantic -g -O0 "${INCLUDES[@]}" "${SOURCES[@]}" -o "$APP" > "$COMPILE_LOG" 2>&1; then
    echo "[BUILD] FAILED. See: $COMPILE_LOG"
    cat "$COMPILE_LOG"
    exit 1
fi

echo "[BUILD] Normal compile passed."

echo "[BUILD] Compiling AddressSanitizer/UBSan executable..."
if ! g++ -std=c++20 -Wall -Wextra -pedantic -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer "${INCLUDES[@]}" "${SOURCES[@]}" -o "$ASAN_APP" > "$ASAN_COMPILE_LOG" 2>&1; then
    echo "[BUILD] ASAN compile FAILED. See: $ASAN_COMPILE_LOG"
    cat "$ASAN_COMPILE_LOG"
    exit 1
fi

echo "[BUILD] ASAN compile passed."

export APP ASAN_APP TEST_DIR SUMMARY DETAILS
python3 <<'PY'
import os
import subprocess
from dataclasses import dataclass
from typing import List, Tuple

APP = os.environ["APP"]
ASAN_APP = os.environ["ASAN_APP"]
TEST_DIR = os.environ["TEST_DIR"]
SUMMARY = os.environ["SUMMARY"]
DETAILS = os.environ["DETAILS"]
CASES_DIR = os.path.join(TEST_DIR, "cases")

@dataclass
class Model:
    heap: List[Tuple[int, int, int, bool]]
    next_id: int
    total_free: int
    strategy: str

    def __init__(self):
        self.heap = []
        self.next_id = 1
        self.total_free = 0
        self.strategy = "FIRST_FIT"

    def fmt_block(self, block):
        bid, start, size, free = block
        if free:
            return f"[free start={start} size={size}]"
        return f"[id={bid} start={start} size={size}]"

    def print_heap(self):
        return "".join(self.fmt_block(b) + " " for b in self.heap)

    def stats(self):
        largest = max((size for _, _, size, free in self.heap if free), default=0)
        frag = 0.0 if self.total_free == 0 else 100.0 * (1.0 - largest / self.total_free)
        return f"Total free: {self.total_free} Largest free: {largest} Fragmentation: {frag:.6g}%"

    def merge_around(self, idx):
        if idx < 0 or idx >= len(self.heap) or not self.heap[idx][3]:
            return
        if idx > 0 and self.heap[idx - 1][3]:
            pid, ps, psz, _ = self.heap[idx - 1]
            _, _, csz, _ = self.heap[idx]
            self.heap[idx - 1] = (pid, ps, psz + csz, True)
            del self.heap[idx]
            idx -= 1
        if idx + 1 < len(self.heap) and self.heap[idx + 1][3]:
            cid, cs, csz, _ = self.heap[idx]
            _, _, nsz, _ = self.heap[idx + 1]
            self.heap[idx] = (cid, cs, csz + nsz, True)
            del self.heap[idx + 1]

    def allocate(self, size):
        if size <= 0:
            raise ValueError("Allocation size must be positive")
        candidates = [(i, b) for i, b in enumerate(self.heap) if b[3] and b[2] >= size]
        if not candidates:
            raise RuntimeError("Not enough memory")
        if self.strategy == "FIRST_FIT":
            idx = candidates[0][0]
        else:
            idx = min(candidates, key=lambda p: p[1][2])[0]
        _, start, old_size, _ = self.heap[idx]
        new_id = self.next_id
        self.next_id += 1
        if old_size == size:
            self.heap[idx] = (new_id, start, size, False)
        else:
            self.heap.insert(idx, (new_id, start, size, False))
            self.heap[idx + 1] = (-1, start + size, old_size - size, True)
        self.total_free -= size

    def run_command(self, raw):
        if raw == "EXIT":
            return ["Goodbye"], True
        parts = raw.split()
        if not parts:
            return ["Empty command"], False
        cmd = parts[0]
        try:
            if cmd == "INIT":
                if len(parts) < 2:
                    return ["INIT requires a size"], False
                if len(parts) > 2:
                    return ["INIT takes exactly one argument"], False
                try: size = int(parts[1])
                except ValueError: return ["INIT requires a size"], False
                if size <= 0:
                    return ["size must be positive"], False
                self.heap = [(-1, 0, size, True)]
                self.next_id = 1
                self.total_free = size
                return [], False
            if cmd == "ALLOC":
                if len(parts) < 2:
                    return ["ALLOC requires a size"], False
                if len(parts) > 2:
                    return ["ALLOC takes exactly one argument"], False
                try: size = int(parts[1])
                except ValueError: return ["ALLOC requires a size"], False
                self.allocate(size)
                return [], False
            if cmd == "FREE":
                if len(parts) < 2:
                    return ["FREE requires an id"], False
                if len(parts) > 2:
                    return ["FREE takes exactly one argument"], False
                try: bid = int(parts[1])
                except ValueError: return ["FREE requires an id"], False
                for i, block in enumerate(self.heap):
                    if block[0] == bid:
                        if block[3]:
                            return ["id already free!"], False
                        _, start, size, _ = block
                        self.heap[i] = (-1, start, size, True)
                        self.total_free += size
                        self.merge_around(i)
                        return [], False
                return ["id does not exist!"], False
            if cmd == "PRINT":
                if len(parts) > 1:
                    return ["PRINT takes no arguments"], False
                return [self.print_heap()], False
            if cmd == "STATS":
                if len(parts) > 1:
                    return ["STATS takes no arguments"], False
                return [self.stats()], False
            if cmd == "STRATEGY":
                if len(parts) < 2:
                    return ["STRATEGY requires strategy name"], False
                if len(parts) > 2:
                    return ["STRATEGY takes exactly one argument"], False
                if parts[1] not in ("FIRST_FIT", "BEST_FIT"):
                    return ["Unknown strategy"], False
                self.strategy = parts[1]
                return [], False
            return ["Unknown command"], False
        except ValueError as e:
            return [str(e)], False
        except RuntimeError as e:
            return [str(e)], False


def expected_output(lines):
    m = Model()
    out = []
    for line in lines:
        lines_out, stop = m.run_command(line)
        out.extend(lines_out)
        if stop:
            break
    return "\n".join(out) + ("\n" if out else "")

cases = []

def add(name, lines, category):
    cases.append({"id": len(cases) + 1, "name": name, "lines": lines, "category": category})

# 1-40 parser and argument validation
invalids = [
    ("empty-line", [""]), ("spaces-only", ["   "]), ("unknown", ["HELLO"]),
    ("lowercase-init", ["init 10"]), ("init-missing", ["INIT"]),
    ("init-nonnumeric", ["INIT abc"]), ("init-extra", ["INIT 10 20"]),
    ("init-zero", ["INIT 0"]), ("init-negative", ["INIT -5"]),
    ("alloc-missing", ["ALLOC"]), ("alloc-nonnumeric", ["ALLOC x"]),
    ("alloc-extra", ["ALLOC 1 2"]), ("alloc-zero", ["ALLOC 0"]),
    ("alloc-negative", ["ALLOC -1"]), ("free-missing", ["FREE"]),
    ("free-nonnumeric", ["FREE x"]), ("free-extra", ["FREE 1 2"]),
    ("print-extra", ["PRINT now"]), ("stats-extra", ["STATS now"]),
    ("strategy-missing", ["STRATEGY"]), ("strategy-extra", ["STRATEGY FIRST_FIT now"]),
    ("strategy-unknown", ["STRATEGY WORST_FIT"]), ("exit-space-not-exit", [" EXIT"]),
    ("tab-empty", ["\t"]), ("mixed-case", ["Init 10"]), ("alloc-before-init", ["ALLOC 1"]),
    ("free-before-init", ["FREE 1"]), ("stats-before-init", ["STATS"]),
    ("print-before-init", ["PRINT"]), ("exact-exit", ["EXIT", "PRINT"]),
    ("init-plus", ["INIT +10", "PRINT"]), ("alloc-plus", ["INIT 10", "ALLOC +3", "PRINT"]),
    ("free-negative-id", ["INIT 10", "FREE -1"]), ("free-zero-id", ["INIT 10", "FREE 0"]),
    ("strategy-lowercase", ["STRATEGY first_fit"]), ("strategy-best-lower", ["STRATEGY best_fit"]),
    ("unknown-after-work", ["INIT 10", "ALLOC 4", "NOPE", "PRINT"]),
    ("init-after-error", ["ALLOC 3", "INIT 6", "PRINT"]),
    ("alloc-too-large", ["INIT 5", "ALLOC 6"]), ("exact-fit", ["INIT 5", "ALLOC 5", "PRINT", "STATS"]),
]
for name, lines in invalids:
    add(name, lines, "parser/errors")

# 41-100 simple first-fit cases
for i in range(60):
    total = 20 + i
    a = 1 + (i % 7)
    b = 2 + (i % 5)
    lines = ["INIT " + str(total), f"ALLOC {a}", f"ALLOC {b}", "PRINT", "STATS"]
    if i % 3 == 0:
        lines += ["FREE 1", "PRINT", "STATS"]
    elif i % 3 == 1:
        lines += ["FREE 2", "PRINT", "STATS"]
    else:
        lines += ["FREE 1", "FREE 2", "PRINT", "STATS"]
    add(f"first-fit-basic-{i+1:02d}", lines, "first-fit/basic")

# 101-160 merge and fragmentation cases
for i in range(60):
    total = 50 + (i % 20)
    a = 5 + (i % 6)
    b = 3 + (i % 8)
    c = 4 + (i % 5)
    lines = [f"INIT {total}", f"ALLOC {a}", f"ALLOC {b}", f"ALLOC {c}"]
    pattern = i % 5
    if pattern == 0:
        lines += ["FREE 2", "PRINT", "STATS"]
    elif pattern == 1:
        lines += ["FREE 1", "FREE 2", "PRINT", "STATS"]
    elif pattern == 2:
        lines += ["FREE 3", "FREE 2", "PRINT", "STATS"]
    elif pattern == 3:
        lines += ["FREE 1", "ALLOC 2", "PRINT", "STATS"]
    else:
        lines += ["FREE 2", f"ALLOC {max(1, b-1)}", "PRINT", "STATS"]
    add(f"merge-fragmentation-{i+1:02d}", lines, "merge/fragmentation")

# 161-220 best-fit / strategy switching
for i in range(60):
    total = 70 + (i % 30)
    a = 8 + (i % 5)
    b = 4 + (i % 7)
    c = 6 + (i % 6)
    d = 2 + (i % 4)
    lines = [f"INIT {total}", f"ALLOC {a}", f"ALLOC {b}", f"ALLOC {c}", "FREE 2"]
    if i % 2 == 0:
        lines += ["STRATEGY BEST_FIT", f"ALLOC {d}", "PRINT", "STATS"]
    else:
        lines += ["STRATEGY BEST_FIT", f"ALLOC {b}", "STRATEGY FIRST_FIT", f"ALLOC {d}", "PRINT", "STATS"]
    add(f"strategy-best-fit-{i+1:02d}", lines, "strategy/best-fit")

# 221-270 reinitialization and ID reset
for i in range(50):
    first = 30 + i
    second = 10 + (i % 15)
    lines = [f"INIT {first}", "ALLOC 5", "ALLOC 6", "PRINT", f"INIT {second}", "PRINT", "ALLOC 3", "PRINT", "STATS"]
    if i % 2 == 0:
        lines += ["FREE 1", "PRINT", "STATS"]
    else:
        lines += ["FREE 2", "PRINT"]
    add(f"reinit-id-reset-{i+1:02d}", lines, "reinit/id-reset")

# 271-300 longer mixed sequences
for i in range(30):
    total = 100 + i * 3
    lines = [f"INIT {total}"]
    for j in range(1, 8):
        lines.append(f"ALLOC {((i + j) % 9) + 1}")
    lines += ["FREE 2", "FREE 4", "STATS"]
    if i % 3 == 0:
        lines += ["STRATEGY BEST_FIT", "ALLOC 3", "ALLOC 5"]
    elif i % 3 == 1:
        lines += ["STRATEGY FIRST_FIT", "ALLOC 2", "FREE 6", "ALLOC 4"]
    else:
        lines += ["STRATEGY BEST_FIT", "ALLOC 1", "FREE 1", "ALLOC 7"]
    lines += ["PRINT", "STATS"]
    add(f"long-mixed-{i+1:02d}", lines, "long/mixed")

assert len(cases) == 300, len(cases)

passed = 0
failed = 0
by_cat = {}
failures = []

with open(DETAILS, "w", encoding="utf-8") as details:
    details.write("Mini Memory Allocator - 300 Test Details\n")
    details.write("=" * 70 + "\n\n")

    for case in cases:
        cid = case["id"]
        name = case["name"]
        cat = case["category"]
        inp = "\n".join(case["lines"]) + "\n"
        exp = expected_output(case["lines"])
        in_path = os.path.join(CASES_DIR, f"test_{cid:03d}_{name}.in")
        exp_path = os.path.join(CASES_DIR, f"test_{cid:03d}_{name}.expected")
        out_path = os.path.join(CASES_DIR, f"test_{cid:03d}_{name}.actual")
        with open(in_path, "w", encoding="utf-8") as f: f.write(inp)
        with open(exp_path, "w", encoding="utf-8") as f: f.write(exp)

        proc = subprocess.run([APP], input=inp, text=True, capture_output=True)
        actual = proc.stdout
        with open(out_path, "w", encoding="utf-8") as f: f.write(actual)
        ok = (proc.returncode == 0 and actual == exp)
        by_cat.setdefault(cat, [0, 0])
        by_cat[cat][1] += 1
        if ok:
            passed += 1
            by_cat[cat][0] += 1
            status = "PASS"
        else:
            failed += 1
            failures.append((cid, name, cat, proc.returncode, in_path, exp_path, out_path, exp, actual, proc.stderr))
            status = "FAIL"

        details.write(f"[{status}] {cid:03d} - {name} ({cat})\n")
        details.write(f"Input file:    {in_path}\n")
        details.write(f"Expected file: {exp_path}\n")
        details.write(f"Actual file:   {out_path}\n")
        if not ok:
            details.write(f"Return code: {proc.returncode}\n")
            details.write("--- INPUT ---\n" + inp)
            details.write("--- EXPECTED ---\n" + exp)
            details.write("--- ACTUAL ---\n" + actual)
            if proc.stderr:
                details.write("--- STDERR ---\n" + proc.stderr)
        details.write("\n")

# Run one ASAN/leak smoke pass over all 300 inputs combined.
combined_input = ""
for c in cases:
    combined_input += "\n".join(c["lines"]) + "\n"
asan_env = os.environ.copy()
asan_env["ASAN_OPTIONS"] = "detect_leaks=1:halt_on_error=1:abort_on_error=1"
asan_env["UBSAN_OPTIONS"] = "halt_on_error=1:abort_on_error=1"
asan = subprocess.run([ASAN_APP], input=combined_input, text=True, capture_output=True, env=asan_env)
asan_stdout = os.path.join(TEST_DIR, "logs", "asan_stdout.log")
asan_stderr = os.path.join(TEST_DIR, "logs", "asan_stderr.log")
with open(asan_stdout, "w", encoding="utf-8") as f: f.write(asan.stdout)
with open(asan_stderr, "w", encoding="utf-8") as f: f.write(asan.stderr)
asan_ok = (asan.returncode == 0 and "ERROR: AddressSanitizer" not in asan.stderr and "runtime error:" not in asan.stderr)

with open(SUMMARY, "w", encoding="utf-8") as s:
    s.write("Mini Memory Allocator - 300 Shell Input Test Summary\n")
    s.write("=" * 70 + "\n")
    s.write(f"Total tests: {len(cases)}\n")
    s.write(f"Passed:      {passed}\n")
    s.write(f"Failed:      {failed}\n")
    s.write(f"ASAN/UBSAN/leak smoke check: {'PASS' if asan_ok else 'FAIL'}\n")
    s.write(f"Details file: {DETAILS}\n")
    s.write(f"Cases folder: {CASES_DIR}\n")
    s.write(f"ASAN stdout:  {asan_stdout}\n")
    s.write(f"ASAN stderr:  {asan_stderr}\n\n")
    s.write("Category breakdown:\n")
    for cat in sorted(by_cat):
        p, t = by_cat[cat]
        s.write(f"  {cat:<24} {p:3d}/{t:3d} passed\n")
    if failures:
        s.write("\nFailures:\n")
        for cid, name, cat, rc, in_path, exp_path, out_path, exp, actual, stderr in failures[:30]:
            s.write(f"  Test {cid:03d} - {name} ({cat}), return code {rc}\n")
            s.write(f"    input:    {in_path}\n")
            s.write(f"    expected: {exp_path}\n")
            s.write(f"    actual:   {out_path}\n")
        if len(failures) > 30:
            s.write(f"  ... and {len(failures) - 30} more failures. See details.txt\n")

print(f"[TESTS] Passed {passed}/{len(cases)}")
print(f"[ASAN]  {'PASS' if asan_ok else 'FAIL'}")
print(f"[SUMMARY] {SUMMARY}")
print(f"[DETAILS] {DETAILS}")
if failed or not asan_ok:
    raise SystemExit(1)
PY

RESULT=$?
if [[ $RESULT -eq 0 ]]; then
    echo "[DONE] All tests passed."
    echo "Summary: $SUMMARY"
else
    echo "[DONE] Some tests failed."
    echo "Summary: $SUMMARY"
    exit $RESULT
fi
