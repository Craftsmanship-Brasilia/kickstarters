// Unit tests: plain functions, no DOM (runs in the "unit" project with the node environment).
import { emptyBoard, statusOf, winnerOf, type Cell } from '../../src/tictactoe/logic';

// Tiny helper so boards read like the real thing: 'XXX......'
const board = (layout: string): Cell[] =>
  [...layout].map((char) => (char === 'X' || char === 'O' ? char : null));

describe('tic tac toe rules', () => {
  it('an empty board has no winner', () => {
    expect(winnerOf(emptyBoard())).toBeNull();
  });

  it.each([
    { layout: 'XXX......', winner: 'X' },
    { layout: 'O..O..O..', winner: 'O' },
    { layout: 'X...X...X', winner: 'X' },
  ])('$layout -> $winner wins', ({ layout, winner }) => {
    expect(winnerOf(board(layout))).toBe(winner);
  });

  it('describes the status', () => {
    expect(statusOf(emptyBoard(), 'X')).toBe('X to play');
    expect(statusOf(board('XOXXOOOXX'), 'O')).toBe('Draw');
  });
});
