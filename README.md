# test_pthread
This is to test how to use pthread in a optimal way

This console based application basically compares when pthread library takes charge of managing CPU assign against when a user explicitly set affinity.  
This results in the former one performs almost same with or a little better than the later one.
This also proves that it is proportional if the number of threads set in N times of CPU cores.
