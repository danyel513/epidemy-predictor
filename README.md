# Epidemic Simulation Project

## Overview

This project is a simulation model to predict the evolution of an infectious disease within a population. It includes both a sequential and a parallel version of the simulation, leveraging `pthreads` to enhance performance for large-scale simulations. The simulation considers the movement and interaction of individuals within a bounded area and tracks infection spread, recovery, and immunity phases over discrete time units.

## Project Structure

### Files and Directories

- **epidemy.c**: Contains core function implementations for updating locations, computing future statuses, and updating current statuses for each person.
- **epidemy.h**: Header file defining the `Person_t` structure and function prototypes.
- **serial_epidemy.c**: The sequential version of the epidemic simulation.
- **parallel_epidemy.c**: The parallel version using `pthreads` for multi-threaded processing.
- **validate_outputs.c**: Utility for comparing outputs from serial and parallel versions. Implemented using the consumer-producer arhitecture.
- **run_tests.sh**: Bash script for compiling, running, and validating simulations with different configurations.

### Code Explanation

#### `epidemy.c`

This file defines functions for:
- **`movePerson()`**: Moves a person based on their direction and amplitude, reversing direction upon reaching the area's boundary.
- **`computeFutureStatus()`**: Determines a person's future status based on their current status and contact with others.
- **`updateStatus()`**: Updates each person's status for the next time step.

#### `epidemy.h`

This header file declares:
- The `Person_t` structure, holding data such as `id`, `coordinates`, `status`, `infectionCounter`, `direction`, and `duration` attributes.
- Prototypes for all simulation-related functions.

## Running the Project

### Prerequisites

Ensure `gcc` and `bash` are installed on your system.

### Compilation

The `run_tests.sh` script automates compilation:

```bash
# Compile shared code and individual executables
./run_tests.sh
