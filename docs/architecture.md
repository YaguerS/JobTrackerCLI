# Architecture

JobTrackerCLI is split into small modules with one clear responsibility each.

## Modules

- `main.c`: program flow and menu actions
- `menu.c`: terminal menu and validated user input
- `application.c`: application data model helpers and dynamic list ownership
- `file.c`: CSV loading and saving
- `stats.c`: statistics calculation and display

## Data Ownership

`ApplicationList` owns the dynamic array of `Application` records. It is initialized once in `main`, passed to menu handlers, and freed before exit.

Individual text fields use fixed-size arrays. This keeps version 1 simple and avoids per-field heap ownership while still enforcing clear input limits.

## Persistence

The program loads `data/applications.csv` on startup and saves it on exit. The Save menu option uses the same persistence path.

CSV fields are quoted when needed, including fields with commas, quotes, or newlines.

## Error Handling

User input is read with `fgets` and parsed explicitly. Invalid input is rejected and the user is prompted again.

File operations return `FileResult` values so callers can print useful messages without depending on low-level file details.
