#!/usr/bin/env python3
"""
GrabX Database Creation Script

This script creates and initializes the SQLite database for the GrabX download manager.
It reads schema.sql to set up the necessary tables for tracking downloads, 
download segments, and application settings.

Author: GrabX Project
"""

import sqlite3
import os
import sys
from enum import IntEnum

# Exit codes for the script
class ExitCode(IntEnum):
    SUCCESS = 0
    FILE_NOT_FOUND = 1
    EXECUTION_ERROR = 2

def get_user_confirmation(message: str) -> bool:
    """
    Get user confirmation with y/n prompt.
    
    Prompts the user with a yes/no question and validates the response.
    Defaults to 'No' if the user just presses Enter.
    
    Args:
        message (str): The confirmation message to display to the user
        
    Returns:
        bool: True if user confirms (y/yes), False otherwise (n/no/empty)
    """
    while True:
        response = input(f"{message} (y/N): ").strip().lower()
        if response in ['y', 'yes']:
            return True
        elif response in ['n', 'no', '']:
            return False
        else:
            print("Please enter 'y' for yes or 'n' for no.")

def create_database(db_path: str, schema_path: str, force_overwrite: bool = False) -> ExitCode:
    """
    Create or recreate the GrabX SQLite database using schema.sql.

    Args:
        db_path (str): Path to SQLite database file.
        schema_path (str): Path to schema.sql file containing CREATE TABLE statements.
        force_overwrite (bool): Overwrite existing DB without asking.

    Returns:
        ExitCode
    """
    try:
        # Check if database already exists
        if os.path.exists(db_path) and not force_overwrite:
            print(f"Database already exists at: {db_path}")
            if not get_user_confirmation("Do you want to overwrite the existing database?"):
                print("Database creation cancelled.")
                return ExitCode.FILE_NOT_FOUND
            else:
                print("Overwriting existing database...")
                os.remove(db_path)

        # Ensure directory exists
        os.makedirs(os.path.dirname(db_path), exist_ok=True)

        print(f"Creating database at: {db_path}")

        # Connect to SQLite
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()

        # Load schema from file
        if not os.path.exists(schema_path):
            print(f"ERROR: Schema file not found at: {schema_path}")
            return ExitCode.FILE_NOT_FOUND

        with open(schema_path, "r", encoding="utf-8") as f:
            schema_sql = f.read()

        print("Applying schema.sql...")
        cursor.executescript(schema_sql)

        # Commit and close
        conn.commit()
        conn.close()

        print(f"✓ Database created successfully at: {db_path}")
        return ExitCode.SUCCESS

    except sqlite3.Error as e:
        print(f"ERROR: SQLite error occurred: {e}")
        return ExitCode.EXECUTION_ERROR
    except OSError as e:
        print(f"ERROR: File system error occurred: {e}")
        return ExitCode.EXECUTION_ERROR
    except Exception as e:
        print(f"ERROR: Unexpected error occurred: {e}")
        return ExitCode.EXECUTION_ERROR

def main() -> int:
    """Main entry point."""
    db_path = "../data/grabx.db"
    schema_path = "schema.sql"   # assumes schema.sql is in the same folder as this script

    print("===== GrabX Database Creation Script =====")
    print(f"Target database: {os.path.abspath(db_path)}")
    print(f"Using schema:   {os.path.abspath(schema_path)}\n")

    result = create_database(db_path, schema_path)

    if result == ExitCode.SUCCESS:
        print("\n" + "="*50)
        print("            DATABASE SCHEMA SUMMARY")
        print("="*50)
        print("📁 downloads table:")
        print("   └── Main download tracking with status, progress, and metadata")
        print("📁 segments table:")
        print("   └── Download segments tracking for parallel/resume downloads")
        print("📁 settings table:")
        print("   └── Application configuration and user preferences")
        print("\n✅ Database is ready for use!")
        print(f"📍 Location: {os.path.abspath(db_path)}")
        return 0
    elif result == ExitCode.FILE_NOT_FOUND:
        return 0
    else:   # unknown error
        print("\n❌ Database creation was not completed.")
        return 1

if __name__ == "__main__":
    sys.exit(main())
