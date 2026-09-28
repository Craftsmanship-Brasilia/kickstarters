import { Game, PositionTakenError, describeStatus } from '../src/tictactoe';
import type { Coordinate } from '../src/tictactoe';

// Helper: play [row, column] pairs from a fresh game.
const play = (...moves: [Coordinate, Coordinate][]): Game =>
  moves.reduce((game, [row, column]) => game.play({ row, column }), Game.start());

describe('Tic Tac Toe', () => {
  it('X plays first', () => {
    expect(Game.start().status).toEqual({ kind: 'in-progress', nextPlayer: 'X' });
  });

  it('players alternate', () => {
    const game = play([0, 0]);

    expect(game.board.at({ row: 0, column: 0 })).toBe('X');
    expect(game.nextPlayer).toBe('O');
  });

  it('does not mutate the previous game', () => {
    const before = Game.start();

    before.play({ row: 1, column: 1 });

    expect(before.board.at({ row: 1, column: 1 })).toBeNull();
  });

  it('rejects a taken position', () => {
    const game = play([0, 0]);

    expect(() => game.play({ row: 0, column: 0 })).toThrow(PositionTakenError);
  });

  it('X wins with a full row', () => {
    const game = play([0, 0], [1, 0], [0, 1], [1, 1], [0, 2]);

    expect(game.status).toEqual({ kind: 'won', winner: 'X' });
    expect(describeStatus(game.status)).toBe('X wins');
  });

  it('O wins with a diagonal', () => {
    const game = play([0, 1], [0, 0], [0, 2], [1, 1], [1, 0], [2, 2]);

    expect(game.status).toEqual({ kind: 'won', winner: 'O' });
  });

  it('full board without a winner is a draw', () => {
    // X O X / X O O / O X X
    const game = play([0, 0], [0, 1], [0, 2], [1, 1], [1, 0], [1, 2], [2, 1], [2, 0], [2, 2]);

    expect(describeStatus(game.status)).toBe('Draw');
  });

  it('cannot play after the game is over', () => {
    const game = play([0, 0], [1, 0], [0, 1], [1, 1], [0, 2]);

    expect(() => game.play({ row: 2, column: 2 })).toThrow('Game is over');
  });
});
