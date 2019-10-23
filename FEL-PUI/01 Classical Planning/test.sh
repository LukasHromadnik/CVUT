#!/bin/bash

for test_folder in depot depot2 driverlog elevators hiking openstacks parcprinter sokoban test transport visitall woodworking
do
    success=0
    strips=`find ./test/$test_folder -name "*.strips"`
    fdr=`find ./test/$test_folder -name "*.fdr"`
    plan=`find ./test/$test_folder -name "*.plan"`
    input=`find ./test/$test_folder -name "*.pddl" -not -name "domain*"`
    ./a.out $strips $fdr > result
    
    result_cost=`grep "cost" result`
    test_cost=`grep "cost" $plan`
    if [ "$result_cost" != "$test_cost" ]; then
        success=1
    fi

    result_h=`grep "h^max" result`
    test_h=`grep "h^max" $plan`
    if [ "$result_h" != "$test_h" ]; then
        success=1
    fi

    ./validate ./test/$test_folder/domain.pddl $input result > /dev/null
    validate_result=$?
    if [ $validate_result -gt 0 ]; then
        success=1
    fi

    if [ $success -gt 0 ]; then
        echo "TEST $test_folder: FAILED"
        if [ "$result_cost" != "$test_cost" ]; then
            echo $result_cost
            echo $test_cost
        fi
        if [ "$result_h" != "$test_h" ]; then
            echo $result_h
            echo $test_h
        fi
        if [ $validate_result -gt 0 ]; then
            echo ";; Validation FAILED"
        fi
    else
        echo "TEST $test_folder: SUCCESS"
    fi
done
rm -f result
