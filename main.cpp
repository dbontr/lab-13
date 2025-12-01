#include <iostream>
#include <vector>
#include <string>

// Original (buggy) function from the lab:
// Counts how many lectures are missed (0 = absent, 1 = present).
// Student fails if they miss 3 or more lectures.
bool fail_lecture(const std::vector<int>& attendance_records) {
    int absent_count = 0;

    // FAULT: starts at i = 1, so it ignores the first lecture at index 0.
    for (int i = 1; i < static_cast<int>(attendance_records.size()); ++i) {
        if (attendance_records[i] == 0) {
            ++absent_count;
        }
    }

    return absent_count >= 3;
}

// Correct reference implementation (used only for testing).
bool fail_lecture_correct(const std::vector<int>& attendance_records) {
    int absent_count = 0;

    for (int i = 0; i < static_cast<int>(attendance_records.size()); ++i) {
        if (attendance_records[i] == 0) {
            ++absent_count;
        }
    }

    return absent_count >= 3;
}

struct TestCase {
    std::string name;
    std::vector<int> records;
    bool describe_only;  // for cases that are “invalid” per spec (Q2)
};

void print_records(const std::vector<int>& v) {
    std::cout << "[";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i + 1 < v.size()) std::cout << ", ";
    }
    std::cout << "]";
}

int main() {
    // Test cases corresponding to the written answers:

    std::vector<TestCase> tests = {
        // Q2: “Does not execute the fault” if size < 2 (invalid per spec, but shown here).
        { "Q2_invalid_size_0", {}, true },
        { "Q2_invalid_size_1", {1}, true },

        // Q3: executes the fault but no error state (first element is 1).
        // [1, 0, 0, 1, 1, 1, 1, 1, 1, 1]
        { "Q3_exec_fault_no_error",
          {1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
          false },

        // Q4: error state but no failure (first element is 0, total absences < 3).
        // [0, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        { "Q4_error_no_failure",
          {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
          false },

        // Q5: failure (first element 0, total absences >= 3).
        // [0, 0, 0, 1, 1, 1, 1, 1, 1, 1]
        { "Q5_failure",
          {0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
          false }
    };

    std::cout << "Running fail_lecture tests\n\n";

    for (const auto& tc : tests) {
        std::cout << "Test: " << tc.name << "\n";
        std::cout << "  attendance_records = ";
        print_records(tc.records);
        std::cout << "\n";

        if (tc.describe_only) {
            std::cout << "  (This test is just to illustrate size < 2; "
                         "it avoids executing the loop / fault but is "
                         "invalid under the spec of 10 lectures.)\n\n";
            continue;
        }

        bool buggy_result   = fail_lecture(tc.records);
        bool oracle_result  = fail_lecture_correct(tc.records);

        std::cout << "  buggy   fail_lecture(...)        = "
                  << (buggy_result ? "FAIL" : "PASS") << "\n";
        std::cout << "  correct fail_lecture_correct(...) = "
                  << (oracle_result ? "FAIL" : "PASS") << "\n";

        if (buggy_result == oracle_result) {
            std::cout << "  RESULT: Same external behavior (no observed failure).\n";
        } else {
            std::cout << "  RESULT: Different behavior → FAILURE exposed.\n";
        }

        std::cout << "\n";
    }

    return 0;
}
