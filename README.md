# House Navigation & Recipe Collection (C Lab)

**Course:** Principles of Programming in C (Year 1)  
**Scenario:** Navigate an 8×8 house grid to collect ingredients in order and bake in the Kitchen.

## Files
- `house_layout.txt` — 8×8 grid of 0/1 (0 = blocked, 1 = path)
- `ingredients_map.txt` — ingredient coordinates (row col), includes `Kitchen`
- `recipes/` — five Scenario A files (names only) and five Scenario B files (name weight)
- `src/` + `include/` — modular C skeleton with TODOs
- `Makefile` — build instructions
- `answers.txt` — written questions (fill in)

## Build
```bash
make
./labhouse ingredients_map.txt recipes/recipeA1_VanillaCake.txt
./labhouse ingredients_map.txt recipes/recipeB1_VanillaCake.txt
```

## Rules
- Move only **UP/DOWN/LEFT/RIGHT** (no diagonals).
- `move(int direction, int steps)` moves stepwise; invalid step = dead-end.
- On dead-end, **restart from Kitchen (0,0)** and try a different path.
- Scenario A: no bag limit.
- Scenario B: each ingredient has a weight. The **bag capacity is set to 60% of the total recipe weight (rounded up)** to force multiple trips. Return to Kitchen to drop off when adding the next item would exceed capacity.

## Visual / UX flags
- `--animate` enable live grid rendering
- `--delay=80` set frame delay in ms (default 120)
- `--quiet` fewer logs, `--debug` more logs
- `--log=run.txt` write event log

**Examples**
```bash
./labhouse ingredients_map.txt recipes/recipeA1_VanillaCake.txt --animate --delay=80
./labhouse ingredients_map.txt recipes/recipeB2_ChocolateCake.txt --animate --delay=100 --log=run.txt
```

## Marking (75 pts)
move() function implementation –15 pts 
collect() function implementation – 20 pts 
File parsing (reading the house layout, ingredients map, and recipes) – 10 pts 
Multiple-trip logic for Scenario B – 10 pts 
Program correctness and code style – 5 pts 
Answers to Q1, Q2, Q3 (in answers.txt) – 5 + 5+ 5pts 


Kitchen is at `(0,0)`. All coordinates are **0-based**.


## Acknowledgements

**Concept:** 
David Sabiiti Bamutura - (dbamutura@must.ac.ug)

**Development:**
Musiimenta Joachim - (musiimentajoachim10@gmail.com)

This laboratory exercise is an original work conceptualized by David Sabiiti Bamutura and developed by Musiimenta Joachim. It was inspired by the idea of using a baking narrative as a creative and relatable approach to explore and demonstrate key C programming concepts.

