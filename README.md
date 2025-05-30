# Othello Game with DxLib

A classic Othello (Reversi) board game implemented in C++ with a graphical user interface using the DxLib library. This version allows you to play against an AI opponent.

This project is based on and extends C code originally provided for a programming class by Professor Tsuyoshi Hashimoto, Information Engineering Department, Matsue National College of Technology, Japan.

## Features

* Classic Othello gameplay.
* Graphical User Interface (GUI) built with DxLib.
* Play against a computer-controlled AI opponent.
* Visual display of the board, pieces, stone counts, and current turn.

## Technologies Used

* C++
* DxLib (for graphics, input, and window management)

## How to Run

1.  **Download the Game:**
    * Navigate to the **[Releases](../../releases)** section of this GitHub repository.
    * Download the latest `.zip` file provided.
2.  **Extract:**
    * Extract the contents of the downloaded `.zip` file into a new folder on your computer.
3.  **Play:**
    * Run the `.exe` file (e.g., `OthelloGame.exe`) found in the extracted folder.
    * Make sure any accompanying files (like `back.bmp` if you're using a custom background image) are in the same folder as the executable.

**Important Note for Windows Users:**
When you run the `.exe` file, Windows SmartScreen might display a warning because the application is not digitally signed by a recognized publisher. This is common for projects from individual developers like us. If you see such a warning, you can typically:
1.  Click on "More info".
2.  Click on "Run anyway".

## Basic Othello Rules

Othello is a strategy game played on an 8x8 board. Two players take turns placing discs with their assigned color (black or white) facing up. When a disc is placed, any of the opponent's discs that form a straight line (horizontal, vertical, or diagonal) between the newly placed disc and another disc of the current player's color are flipped to the current player's color. The game ends when no more legal moves can be made by either player. The player with the most discs of their color on the board wins.

---

*The foundational Othello logic and C structure were provided as teaching material by Professor Tsuyoshi Hashimoto.*
