// With "jsx": "react-jsx" (React 17+) you don't need `import React from 'react'` for JSX.
import { useState } from 'react';
import { emptyBoard, statusOf, winnerOf, type Cell, type Player } from './logic';

export function TicTacToe() {
  // useState<T>: typed state. The lazy initializer `() => ...` runs only on the first render.
  const [cells, setCells] = useState<Cell[]>(() => emptyBoard());
  const [next, setNext] = useState<Player>('X');

  // Derived values: computed on every render instead of being stored in state.
  const winner = winnerOf(cells);

  function play(index: number) {
    setCells(cells.with(index, next)); // new array: React state must be treated as immutable
    setNext(next === 'X' ? 'O' : 'X');
  }

  function reset() {
    setCells(emptyBoard());
    setNext('X');
  }

  return (
    <section aria-labelledby="tictactoe-title">
      <h2 id="tictactoe-title">Tic Tac Toe</h2>

      {/* role="status" makes it an accessible live region; tests find it with getByRole('status') */}
      <p role="status">{statusOf(cells, next)}</p>

      <div role="group" aria-label="board" className="board">
        {cells.map((cell, index) => (
          <button
            key={index}
            type="button"
            // accessible name used by tests and screen readers: "row 1 column 1"
            aria-label={`row ${Math.floor(index / 3) + 1} column ${(index % 3) + 1}`}
            disabled={cell !== null || winner !== null}
            onClick={() => play(index)}
          >
            {cell}
          </button>
        ))}
      </div>

      <button type="button" onClick={reset}>
        New game
      </button>
    </section>
  );
}
