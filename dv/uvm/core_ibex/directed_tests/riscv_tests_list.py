import os

def append_directed_testlist(isa_tests):
    testlist_string = ""

    for isa_test in isa_tests:
        available_isa_tests = os.popen('ls '+'../../../../vendor/riscv-tests/isa/'+isa_test+' | egrep .S').read()
        available_isa_testlist = []
        for test in available_isa_tests.split('\n')[:-1]:
            available_isa_testlist.append(test)
        for test_name_str in available_isa_testlist:
            test_name = test_name_str.split('.')[0]
            testlist_string = testlist_string + '''
- test: {test_name}
  desc: >
    riscv isa test - {test_name}
  iterations: 1
  test_srcs: ../../../../vendor/riscv-tests/isa/{isa_test}/{test_name}.S
  config: base
    '''.format(test_name = test_name, isa_test = isa_test)
    with open('directed_testlist.yaml', "a") as f:
        f.write(testlist_string)
    print(testlist_string)

def list_tests(dir):
    testlist_str = os.popen('ls '+dir).read()
    testlist = []
    for test in testlist_str.split('\n')[:-1]:
        testlist.append(test)
        print(testlist)
    return testlist

isa_tests = {'rv32mi', 'rv32uc', 'rv32ui', 'rv32um'}
append_directed_testlist(isa_tests)
