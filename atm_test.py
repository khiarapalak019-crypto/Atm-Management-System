"""Black-box regression test for the ATM Management System.

Pass the path to an already-built ATM executable when invoking this script.
The test runs it in a temporary directory, so the workspace data files are
left unchanged.
"""

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


class ATMProgramTest(unittest.TestCase):
    executable = None

    def test_add_delete_check_report_save_and_exit(self):
        with tempfile.TemporaryDirectory(prefix="atm_test_") as temp_dir:
            work = Path(temp_dir)
            (work / "ACCOUNTS.TXT").write_text(
                "1001|ABC|20000\n", encoding="utf-8"
            )
            (work / "CARDS.TXT").write_text(
                "CARD1001|1001\n", encoding="utf-8"
            )
            (work / "TRANS.TXT").write_text(
                "DEPOSIT 1001 1001 20000\n", encoding="utf-8"
            )

            # Add account 1002, attempt to delete funded account 1001,
            # check 1002's balance, view the report, save, and exit.
            supplied_input = (
                "1\nABC\n30000\n"
                "5\n1001\n"
                "6\n1002\n1234\n1\n"
                "7\n"
                "8\n1\n"
                "9\n"
            )
            result = subprocess.run(
                [str(self.executable)],
                cwd=work,
                input=supplied_input,
                text=True,
                capture_output=True,
                timeout=15,
                check=False,
            )

            output = result.stdout + result.stderr
            self.assertEqual(
                result.returncode,
                0,
                msg=f"ATM exited with code {result.returncode}.\n{output}",
            )
            self.assertIn("Account ID: 1002", output)
            self.assertIn("Withdraw all money before deletion.", output)
            self.assertIn("Current Balance: Rs. 30000", output)
            self.assertIn("Total accounts: 2", output)
            self.assertIn("Total balance: Rs. 50000", output)
            self.assertIn("Thank you for using the ATM.", output)

            self.assertEqual(
                (work / "ACCOUNTS.TXT").read_text(encoding="utf-8"),
                "1001|ABC|20000\n1002|ABC|30000\n",
            )
            self.assertEqual(
                (work / "CARDS.TXT").read_text(encoding="utf-8"),
                "CARD1001|1001\nCARD1002|1002\n",
            )
            self.assertEqual(
                (work / "TRANS.TXT").read_text(encoding="utf-8"),
                "DEPOSIT 1001 1001 20000\nDEPOSIT 1002 1002 30000\n",
            )
            self.assertTrue((work / "ATM_DATA.TXT").is_file())


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("Provide the path to the existing ATM executable.")

    ATMProgramTest.executable = Path(sys.argv[1]).expanduser().resolve()
    if not ATMProgramTest.executable.is_file():
        sys.exit(f"ATM executable not found: {ATMProgramTest.executable}")

    # Remove the executable path before unittest parses its own arguments.
    sys.argv = [sys.argv[0]]
    unittest.main()
