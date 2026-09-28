// Test doubles in Vitest: `vi.*` (the Jest version of this file uses `jest.*`).
import { Board, Game } from '../src/tictactoe';

describe('test doubles (Vitest)', () => {
  afterEach(() => {
    vi.restoreAllMocks(); // undo spies after each test
    vi.useRealTimers();
  });

  it('vi.fn creates a mock function', () => {
    const onMove = vi.fn<(player: string) => void>();

    onMove('X');

    expect(onMove).toHaveBeenCalledWith('X');
    expect(onMove).toHaveBeenCalledTimes(1);
  });

  it('vi.fn can return values', () => {
    const random = vi.fn().mockReturnValueOnce(0).mockReturnValue(2);

    expect([random(), random(), random()]).toEqual([0, 2, 2]);
  });

  it('vi.spyOn wraps a real method', () => {
    const spy = vi.spyOn(Board.prototype, 'winner').mockReturnValue('O');

    expect(Game.start().status).toEqual({ kind: 'won', winner: 'O' });
    expect(spy).toHaveBeenCalled();
  });

  it('fake timers control time', () => {
    vi.useFakeTimers();
    const callback = vi.fn();

    setTimeout(callback, 1000);
    vi.advanceTimersByTime(1000);

    expect(callback).toHaveBeenCalled();
  });
});
