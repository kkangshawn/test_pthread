# test_pthread

This is to test how to use pthread in an optimal way

This console application compares when pthread library takes charge of managing CPU assign against when a user explicitly set affinity.  
This results in the former one performs almost same with or a little better than the later one.
This also proves that the performance is proportional if the number of threads is set N times of CPU cores.
