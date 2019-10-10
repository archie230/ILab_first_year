#pragma once
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>

namespace tests_framework {
//  test void function(); should call Assert or throw std::logic_error exception if test failed

    typedef void (*testfunction_t)();
    typedef std::vector<std::pair<testfunction_t, std::string>> vector_with_tests;

    template<typename T, typename U>
    void Assert(const T &expr, const U &output, unsigned int LINE) {
        if (expr != output) {
            std::stringstream ss;
            ss << "Assertion failed. Line: " << LINE;
            throw std::logic_error(ss.str());
        }
    }

    const int TESTS_FAILED_STATUS = -230;

    class test_framework {
    public:
        void operator()(const std::vector<std::pair<testfunction_t, std::string>>& tests_vector) {
            for(auto& pair : tests_vector)
                RunTest(pair.first, pair.second);

            if (tests_failed > 0) {
                std::cerr << "failed: " << tests_failed << " tests.Terminate\n";
                exit(TESTS_FAILED_STATUS);
            }
        }

        void RunTest(testfunction_t test, const std::string &test_name) {
            try {
                test();
                std::cerr << "Test " << test_name << " OK\n";
            }

            catch (const std::logic_error &exc) {
                std::cerr << exc.what() << std::endl;
                std::cerr << "Test " << test_name << " FAILED\n";
                ++tests_failed;
            }
        }

    private:
        int tests_failed = 0;
    };
}