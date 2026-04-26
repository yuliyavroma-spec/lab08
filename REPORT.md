## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

## Report
```bash
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..
$ git add third-party/gtest
$ git commit -m"added gtest framework"
```
```bash
$ mkdir tests
```
```bash
$ cmake -H. -B_build -DBUILD_TESTS=ON
$ cmake --build _build
$ _build/check
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from Print
[ RUN      ] Print.InFileStream
[       OK ] Print.InFileStream (0 ms)
[----------] 1 test from Print (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.

```
```bash
$ mkdir artifacts
```
```bash
$ sleep 20s && gnome-screenshot --file artifacts/screenshot.png

** Message: 20:23:35.497: Unable to use GNOME Shell's builtin screenshot interface, resorting to fallback X11.

```
## Homework

### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.
```bash
$ cat > CMakeLists.txt << 'EOF'

cmake_minimum_required(VERSION 3.14)
project(banking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(COVERAGE "Enable coverage reporting" OFF)

add_library(banking STATIC
    src/Account.cpp
    src/Transaction.cpp
)

target_include_directories(banking PUBLIC include)

if(COVERAGE)
    target_compile_options(banking PRIVATE --coverage -O0 -g)
    target_link_libraries(banking PRIVATE --coverage)
endif()

if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    
    add_executable(banking_tests
        tests/test_account.cpp
        tests/test_transaction.cpp
        tests/mock_account.hpp
    )
    
    target_link_libraries(banking_tests banking gtest_main gmock)
    target_include_directories(banking_tests PRIVATE include)
    add_test(NAME banking_tests COMMAND banking_tests)
endif()
EOF
```
2. Создайте модульные тесты на классы `Transaction` и `Account`.

```bash
$ cat > include/Account.hpp << 'EOF'

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

class Account {
private:
    std::string owner;
    double balance;
    bool locked;

public:
    Account(const std::string& owner, double initial_balance = 0);
    virtual ~Account() = default;
    
    virtual const std::string& GetOwner() const;
    virtual double GetBalance() const;
    virtual void ChangeBalance(double amount);
    virtual void Lock();
    virtual void Unlock();
    virtual bool IsLocked() const;
};

#endif
EOF
```

```bash
$ cat > src/Account.cpp << 'EOF'
 
 #include "Account.hpp"

Account::Account(const std::string& owner, double initial_balance)
    : owner(owner), balance(initial_balance), locked(false) {}

const std::string& Account::GetOwner() const {
    return owner;
}

double Account::GetBalance() const {
    return balance;
}

void Account::ChangeBalance(double amount) {
    if (!locked) {
        balance += amount;
    }
}

void Account::Lock() {
    locked = true;
}

void Account::Unlock() {
    locked = false;
}

bool Account::IsLocked() const {
    return locked;
}
EOF
```

```bash
$ cat > include/Transaction.hpp << 'EOF'

 #ifndef TRANSACTION_HPP
 #define TRANSACTION_HPP

 #include <string>
 #include "Account.hpp"

class Transaction {
private:
    double amount;
    std::string sender_id;
    std::string receiver_id;
    bool executed;
    bool rolled_back;

public:
    Transaction(double amount, const std::string& sender, const std::string& receiver);
    
    bool Execute(Account& from, Account& to);
    bool Rollback();
    bool IsExecuted() const;
    bool IsRolledBack() const;
};

 #endif
EOF
```
```bash
$ cat > src/Transaction.cpp << 'EOF'

 #include "Transaction.hpp"

Transaction::Transaction(double amount, const std::string& sender, const std::string& receiver)
    : amount(amount), sender_id(sender), receiver_id(receiver), executed(false), rolled_back(false) {}

bool Transaction::Execute(Account& from, Account& to) {
    if (executed || rolled_back) {
        return false;
    }
    
    if (from.IsLocked() || to.IsLocked()) {
        return false;
    }
    
    if (from.GetBalance() < amount) {
        return false;
    }
    
    from.ChangeBalance(-amount);
    to.ChangeBalance(amount);
    executed = true;
    return true;
}

bool Transaction::Rollback() {
    if (!executed || rolled_back) {
        return false;
    }
    rolled_back = true;
    executed = false;
    return true;
}

bool Transaction::IsExecuted() const {
    return executed;
}

bool Transaction::IsRolledBack() const {
    return rolled_back;
}
EOF
```
    * Используйте mock-объекты.

```bash 
$ cat > tests/mock_account.hpp << 'EOF'

#ifndef MOCK_ACCOUNT_HPP
#define MOCK_ACCOUNT_HPP

#include <gmock/gmock.h>
#include "Account.hpp"

class MockAccount : public Account {
public:
    MockAccount(const std::string& owner, double balance) : Account(owner, balance) {}
    
    ~MockAccount() override = default;
    
    MOCK_METHOD(const std::string&, GetOwner, (), (const));
    MOCK_METHOD(double, GetBalance, (), (const));
    MOCK_METHOD(void, ChangeBalance, (double amount));
    MOCK_METHOD(void, Lock, ());
    MOCK_METHOD(void, Unlock, ());
    MOCK_METHOD(bool, IsLocked, (), (const));
};

#endif
EOF
```
   * Тесты.

```bash
$ cat > tests/test_account.cpp << 'EOF'

 #include <gtest/gtest.h>
 #include "Account.hpp"

TEST(AccountTest, ConstructorInitializesCorrectly) {
    Account acc("John Doe", 1000.0);
    
    EXPECT_EQ(acc.GetOwner(), "John Doe");
    EXPECT_DOUBLE_EQ(acc.GetBalance(), 1000.0);
    EXPECT_FALSE(acc.IsLocked());
}

TEST(AccountTest, ConstructorDefaultBalance) {
    Account acc("Jane Doe");
    
    EXPECT_EQ(acc.GetOwner(), "Jane Doe");
    EXPECT_DOUBLE_EQ(acc.GetBalance(), 0.0);
}

TEST(AccountTest, ChangeBalanceWhenUnlocked) {
    Account acc("Test", 500.0);
    
    acc.ChangeBalance(200.0);
    EXPECT_DOUBLE_EQ(acc.GetBalance(), 700.0);
    
    acc.ChangeBalance(-100.0);
    EXPECT_DOUBLE_EQ(acc.GetBalance(), 600.0);
}

TEST(AccountTest, ChangeBalanceWhenLockedDoesNothing) {
    Account acc("Test", 500.0);
    acc.Lock();
    
    acc.ChangeBalance(200.0);
    EXPECT_DOUBLE_EQ(acc.GetBalance(), 500.0);
}

TEST(AccountTest, LockUnlock) {
    Account acc("Test", 100.0);
    
    EXPECT_FALSE(acc.IsLocked());
    acc.Lock();
    EXPECT_TRUE(acc.IsLocked());
    acc.Unlock();
    EXPECT_FALSE(acc.IsLocked());
}
EOF
```

```bash
$ cat > tests/test_transaction.cpp << 'EOF'

 #include <gtest/gtest.h>
 #include <gmock/gmock.h>
 #include "mock_account.hpp"
 #include "Transaction.hpp"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;

class TransactionTest : public ::testing::Test {
protected:
    NiceMock<MockAccount>* from;
    NiceMock<MockAccount>* to;
    
    void SetUp() override {
        from = new NiceMock<MockAccount>("Alice", 1000.0);
        to = new NiceMock<MockAccount>("Bob", 500.0);
    }
    
    void TearDown() override {
        delete from;
        delete to;
    }
};

TEST_F(TransactionTest, ExecuteValidTransaction) {
    EXPECT_CALL(*from, GetBalance()).WillOnce(Return(1000.0));
    EXPECT_CALL(*from, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*to, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*from, ChangeBalance(-300.0)).Times(1);
    EXPECT_CALL(*to, ChangeBalance(300.0)).Times(1);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_TRUE(txn.Execute(*from, *to));
    EXPECT_TRUE(txn.IsExecuted());
}

TEST_F(TransactionTest, ExecuteFailsWhenSenderLocked) {
    EXPECT_CALL(*from, IsLocked()).WillOnce(Return(true));
    EXPECT_CALL(*from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(*to, ChangeBalance(_)).Times(0);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_FALSE(txn.Execute(*from, *to));
    EXPECT_FALSE(txn.IsExecuted());
}

TEST_F(TransactionTest, ExecuteFailsWhenReceiverLocked) {
    EXPECT_CALL(*from, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*to, IsLocked()).WillOnce(Return(true));
    EXPECT_CALL(*from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(*to, ChangeBalance(_)).Times(0);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_FALSE(txn.Execute(*from, *to));
}

TEST_F(TransactionTest, ExecuteFailsWithInsufficientFunds) {
    EXPECT_CALL(*from, GetBalance()).WillOnce(Return(100.0));
    EXPECT_CALL(*from, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*to, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(*to, ChangeBalance(_)).Times(0);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_FALSE(txn.Execute(*from, *to));
}

TEST_F(TransactionTest, CannotExecuteTwice) {
    EXPECT_CALL(*from, GetBalance()).WillOnce(Return(1000.0));
    EXPECT_CALL(*from, IsLocked()).WillRepeatedly(Return(false));
    EXPECT_CALL(*to, IsLocked()).WillRepeatedly(Return(false));
    EXPECT_CALL(*from, ChangeBalance(-300.0)).Times(1);
    EXPECT_CALL(*to, ChangeBalance(300.0)).Times(1);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_TRUE(txn.Execute(*from, *to));
    EXPECT_FALSE(txn.Execute(*from, *to));
}

TEST_F(TransactionTest, RollbackAfterExecution) {
    EXPECT_CALL(*from, GetBalance()).WillOnce(Return(1000.0));
    EXPECT_CALL(*from, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*to, IsLocked()).WillOnce(Return(false));
    EXPECT_CALL(*from, ChangeBalance(-300.0)).Times(1);
    EXPECT_CALL(*to, ChangeBalance(300.0)).Times(1);
    
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_TRUE(txn.Execute(*from, *to));
    EXPECT_TRUE(txn.Rollback());
    EXPECT_TRUE(txn.IsRolledBack());
}

TEST_F(TransactionTest, CannotRollbackWithoutExecution) {
    Transaction txn(300.0, "Alice", "Bob");
    EXPECT_FALSE(txn.Rollback());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
EOF
```

   * Покрытие кода должно составлять 100%.
```bash
$ cmake -B build -DBUILD_TESTS=ON

-- Configuring done (0.7s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/yuliyavroma-spec/workspace/projects/lab05/banking/build
```
```bash
$ cmake --build build

[ 21%] Built target banking
[ 35%] Built target gtest
[ 50%] Built target gmock
[ 64%] Built target gtest_main
[ 85%] Built target banking_tests
[100%] Built target gmock_main
```
```bash
$ ./build/banking_tests

[==========] Running 12 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 5 tests from AccountTest
[ RUN      ] AccountTest.ConstructorInitializesCorrectly
[       OK ] AccountTest.ConstructorInitializesCorrectly (0 ms)
[ RUN      ] AccountTest.ConstructorDefaultBalance
[       OK ] AccountTest.ConstructorDefaultBalance (0 ms)
[ RUN      ] AccountTest.ChangeBalanceWhenUnlocked
[       OK ] AccountTest.ChangeBalanceWhenUnlocked (0 ms)
[ RUN      ] AccountTest.ChangeBalanceWhenLockedDoesNothing
[       OK ] AccountTest.ChangeBalanceWhenLockedDoesNothing (0 ms)
[ RUN      ] AccountTest.LockUnlock
[       OK ] AccountTest.LockUnlock (0 ms)
[----------] 5 tests from AccountTest (0 ms total)

[----------] 7 tests from TransactionTest
[ RUN      ] TransactionTest.ExecuteValidTransaction
[       OK ] TransactionTest.ExecuteValidTransaction (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsWhenSenderLocked
[       OK ] TransactionTest.ExecuteFailsWhenSenderLocked (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsWhenReceiverLocked
[       OK ] TransactionTest.ExecuteFailsWhenReceiverLocked (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsWithInsufficientFunds
[       OK ] TransactionTest.ExecuteFailsWithInsufficientFunds (0 ms)
[ RUN      ] TransactionTest.CannotExecuteTwice
[       OK ] TransactionTest.CannotExecuteTwice (0 ms)
[ RUN      ] TransactionTest.RollbackAfterExecution
[       OK ] TransactionTest.RollbackAfterExecution (0 ms)
[ RUN      ] TransactionTest.CannotRollbackWithoutExecution
[       OK ] TransactionTest.CannotRollbackWithoutExecution (0 ms)
[----------] 7 tests from TransactionTest (0 ms total)

[----------] Global test environment tear-down
[==========] 12 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 12 tests.
```

```bash
$ lcov --capture --directory . --output-file coverage.info
$ lcov --remove coverage.info '/usr/*' '*/gtest/*' '*/build/*' --output-file coverage.info
$ lcov --list coverage.info

                      |Lines       |Functions  
Filename              |Rate     Num|Rate    Num
===============================================
[/home/yulia/yuliyavroma-spec/workspace/projects/lab05/banking/src/]
Account.cpp           | 100%     18| 100%     7
Transaction.cpp       | 100%     23| 100%     5
===============================================
                Total:| 100%     41| 100%    12
Message summary:
  no messages were reported

```

3. Настройте сборочную процедуру на **GitHub Actions**.

```bash
$ cat > .github/workflows/ci.yml << 'EOF'

name: CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
      with:
        submodules: recursive
    
    - name: Configure CMake
      run: cmake -B build -DBUILD_TESTS=ON
      working-directory: banking
    
    - name: Build
      run: cmake --build build
      working-directory: banking
    
    - name: Run tests
      run: ./build/banking_tests
      working-directory: banking
EOF
```

4. Настройте [Coveralls.io](https://coveralls.io/).


```bash
$ cat > .github/workflows/coverage.yml << 'EOF'
 
name: Coverage

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  coverage:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
      with:
        submodules: recursive

    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y lcov g++-10 cmake

    - name: Configure
      run: cmake -B build -DBUILD_TESTS=ON -DCOVERAGE=ON
      working-directory: banking
     - name: Generate coverage report
      run: |
        lcov --capture --directory . --output-file coverage.info
        lcov --remove coverage.info '/usr/*' '*/gtest/*' '*/tests/*' --output-file coverage.info
        lcov --list coverage.info
      working-directory: banking

    - name: Upload to Coveralls
      uses: coverallsapp/github-action@v2
      with:
        file: banking/coverage.info
        github-token: ${{ secrets.GITHUB_TOKEN }}
 EOF
```
