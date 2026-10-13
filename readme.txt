ATM Management System — Python Black-Box Test
===============================================

Files
-----
atm_test.py  Python regression test for the C++ ATM program.

Purpose
-------
The test starts from one funded account (ID 1001), then feeds the same
sequence of actions represented in the supplied run output:

  - Add account 1002 with an opening balance of Rs. 30000
  - Attempt to delete account 1001, which must be rejected because it has a balance
  - Check the balance of account 1002 using PIN 1234
  - Display the report, save, and exit

It checks the program's messages and verifies the persisted ACCOUNTS.TXT,
CARDS.TXT, and TRANS.TXT data, as well as creation of ATM_DATA.TXT.

Requirements and setup
----------------------
Python 3 and an already-built executable of the C++ ATM program are required.
The test script does not compile the C++ source. Supply the executable's path
when launching the Python test. The executable may be a Windows .exe or another
executable supported by the current operating system.

Safety
------
The test uses a temporary working directory and creates its own initial data
there, so it does not overwrite the account and transaction files in the
project workspace. The temporary files are removed after the test completes.
