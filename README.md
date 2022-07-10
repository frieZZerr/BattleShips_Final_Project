![battleships](https://user-images.githubusercontent.com/105242009/178099044-d89cc434-d49b-4452-bc3c-0f95852191b1.jpg)

# Rules

### Basics

1. 2-player strategy turn type game based on guessing opponent ships' positions.
2. There are 2 boards with `size = 10x10`: one with player's ships and other with player's guessed positions.
3. Players guess the `coordinates` of opponent's ships.
4. Player who manages to destroy all opponent's ships wins.

### Board

Each board is 2D and has letters `A-J` on the X-axis and numbers `1-10` on Y-axis:

![boards](https://user-images.githubusercontent.com/105242009/178139939-d3f479b7-d0a4-4bc9-a1fe-3142ddbe0344.JPG)

First one displays ***player's ships*** and `seamine` and the other guessed coordinates.

### Ships

There are ***4 types*** of battleships:

- `Destroyer`  --> _size=1, amount=4_
- `Cruiser`    --> _size=2, amount=3_
- `Battleship` --> _size=3, amount=2_
- `Carrier`    --> _size=4, amount=1_

### `Coordinates` input

- ✔️ `C6`
- ✔️ `g3`
- ❌ `K15`

### Seamine

Every player has a `seamine`. It's an object that generates ***only in the specified area***:

![seamine](https://user-images.githubusercontent.com/105242009/178140409-11b51df2-fc45-41d9-bc50-ed4447b39543.jpg)

If the current player shoots at the _enemy's seamine coordinates_ it explodes, destroying **everything** in it's explosion radius that is `3x3`.
If the seamine is **really close** to a ship and the ship gets destroyed, it also triggers the explosion of this seamine.

### Victory condition

The game ends if one of the players manages to ***destroy all of the enemy's ships***.
