All program were compiled and tested with GCC with the -pthead argument.

[Phase 1]
Creates a set number of accounts, threads and the number of transactions per thread, all with constant values that can be changed before compilation. Run variables -d and -w will change the program from deposit mode (default) to withdraw mode. This program will produce a race condition.

[Phase 2]
All previous information from phase 1 is applicable here, however phase 2 will use mutex locks to ensure thread safe access to account balances. The program will also display how much time (in seconds) the program took to create and perform all of the threaded work.

[Phase 3]
Creates a set number of accounts and threads initialized at compile time. The program will create a circular set of transactions equal to the number of threads denoted at compile time. The circular processing of transactions will ensure that a deadlock scenario will occur.

[Phase 4]
Everything from phase 3 applies here, however this time the transaction method uses a lock ordering deadlock resolution to ensure that no deadlock will occur, even when all the conditions of a deadlock are present.
