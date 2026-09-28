import { Board } from './board';
import type { GameStatus, Player, Position, WinMessage } from './types';

export class Game {
  // Parameter properties: `readonly board` declares AND assigns the field.
  // (Not allowed with "erasableSyntaxOnly", which Node's built-in TS support requires.)
  private constructor(
    readonly board: Board,
    readonly nextPlayer: Player,
  ) {}

  static start(): Game {
    return new Game(Board.empty(), 'X');
  }

  get status(): GameStatus {
    const winner = this.board.winner();
    if (winner) return { kind: 'won', winner };
    if (this.board.isFull) return { kind: 'draw' };
    return { kind: 'in-progress', nextPlayer: this.nextPlayer };
  }

  play(position: Position): Game {
    if (this.status.kind !== 'in-progress') throw new Error('Game is over');
    const opponent: Player = this.nextPlayer === 'X' ? 'O' : 'X';
    return new Game(this.board.place(this.nextPlayer, position), opponent);
  }
}

// Exhaustive switch over a discriminated union: if a new `kind` is added,
// `const unreachable: never = status` stops compiling.
export function describeStatus(status: GameStatus): WinMessage | 'Draw' | `${Player} to play` {
  switch (status.kind) {
    case 'won':
      return `${status.winner} wins`; // TS knows `status.winner` exists here (narrowing)
    case 'draw':
      return 'Draw';
    case 'in-progress':
      return `${status.nextPlayer} to play`;
    default: {
      const unreachable: never = status;
      return unreachable;
    }
  }
}
