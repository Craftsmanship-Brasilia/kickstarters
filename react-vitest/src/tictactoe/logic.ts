// Pure game rules, no React: easy to unit test (tests/unit) and reuse.
export type Player = 'X' | 'O';
export type Cell = Player | null;

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

export const emptyBoard = (): Cell[] => Array<Cell>(9).fill(null);

export function winnerOf(cells: readonly Cell[]): Player | null {
  for (const [a, b, c] of LINES) {
    const player = cells[a];
    if (player && player === cells[b] && player === cells[c]) return player;
  }
  return null;
}

export function statusOf(cells: readonly Cell[], next: Player): string {
  const winner = winnerOf(cells);
  if (winner) return `${winner} wins`;
  if (cells.every((cell) => cell !== null)) return 'Draw';
  return `${next} to play`;
}
