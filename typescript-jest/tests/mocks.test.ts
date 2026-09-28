// Test doubles in Jest: `jest.*` (the Vitest version of this file uses `vi.*`).
import { Board, Game } from '../src/tictactoe';

describe('test doubles (Jest)', () => {
  afterEach(() => {
    jest.restoreAllMocks(); // undo spies after each test
    jest.useRealTimers();
  });

  it('jest.fn creates a mock function', () => {
    const onMove = jest.fn();

    onMove('X');

    expect(onMove).toHaveBeenCalledWith('X');
    expect(onMove).toHaveBeenCalledTimes(1);
  });

  it('jest.fn can return values', () => {
    const random = jest.fn().mockReturnValueOnce(0).mockReturnValue(2);

    expect([random(), random(), random()]).toEqual([0, 2, 2]);
  });

  it('jest.spyOn wraps a real method', () => {
    const spy = jest.spyOn(Board.prototype, 'winner').mockReturnValue('O');

    expect(Game.start().status).toEqual({ kind: 'won', winner: 'O' });
    expect(spy).toHaveBeenCalled();
  });

  it('fake timers control time', () => {
    jest.useFakeTimers();
    const callback = jest.fn();

    setTimeout(callback, 1000);
    jest.advanceTimersByTime(1000);

    expect(callback).toHaveBeenCalled();
  });
});
