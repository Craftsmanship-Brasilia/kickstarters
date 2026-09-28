// Types only — this file produces no JavaScript at all.

// Union of string literals instead of an enum (idiomatic modern TS, zero runtime cost).
export type Player = 'X' | 'O';

export type Cell = Player | null;

// Union of number literals: row/column can only be 0, 1 or 2.
export type Coordinate = 0 | 1 | 2;

export type Position = { readonly row: Coordinate; readonly column: Coordinate };

// Discriminated union: the `kind` field tells TS which shape you have.
export type GameStatus =
  | { readonly kind: 'in-progress'; readonly nextPlayer: Player }
  | { readonly kind: 'won'; readonly winner: Player }
  | { readonly kind: 'draw' };

// Template literal type: 'X wins' | 'O wins'
export type WinMessage = `${Player} wins`;
