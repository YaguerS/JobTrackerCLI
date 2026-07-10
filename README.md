# JobTrackerCLI

JobTrackerCLI is a C17 command-line application for tracking job applications during an IT job search.

## Features

- Add job applications
- View all applications
- Update application status
- Search by company
- Display job-search statistics
- Load and save data as CSV

## Data

Application data is stored in:

```bash
data/applications.csv
```

The file is created automatically when the program saves.

## Build

```bash
make
```

## Run

```bash
make run
```

## Clean

```bash
make clean
```

## Development Notes

- Language: C17
- Compiler: GCC
- Dependencies: standard C library only
- Main warning target: `-Wall -Wextra -Wpedantic`
- Runtime data is ignored by Git so personal job-search records are not committed accidentally.
