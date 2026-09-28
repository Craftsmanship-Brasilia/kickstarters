// `import type` is erased at compile time — use it for things only used as types.
// With "verbatimModuleSyntax" TS forces you to be explicit about this.
import type { Cell, Player, Position } from './types';
import { PositionTakenError } from './errors';

const LINES = [
  [0, 1, 2],
  [3, 4, 5],
  [6, 7, 8],
  [0, 3, 6],
  [1, 4, 7],
  [2, 5, 8],
  [0, 4, 8],
  [2, 4, 6],
] as const;

const indexOf = ({ row, column }: Position): number => row * 3 + column;

// Immutable board: place() returns a new Board.
export class Board {
  // `#cells` is a real JavaScript private field (enforced at runtime).
  // `private` (TS keyword) is only checked at compile time.
  readonly #cells: readonly Cell[];

  // Private constructor: callers must use Board.empty().
  private constructor(cells: readonly Cell[]) {
    this.#cells = cells;
  }

  static empty(): Board {
    return new Board(Array<Cell>(9).fill(null));
  }

  at(position: Position): Cell {
    return this.#cells[indexOf(position)] ?? null;
  }

  place(player: Player, position: Position): Board {
    if (this.at(position) !== null) throw new PositionTakenError(position);
    // Array.prototype.with (ES2023): copy of the array with one element replaced.
    return new Board(this.#cells.with(indexOf(position), player));
  }

  // Getter: accessed like a property (`board.isFull`).
  get isFull(): boolean {
    return this.#cells.every((cell) => cell !== null);
  }

  winner(): Player | null {
    for (const [a, b, c] of LINES) {
      const player = this.#cells[a];
      if (player && player === this.#cells[b] && player === this.#cells[c]) return player;
    }
    return null;
  }
}
