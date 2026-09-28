// `describe`, `it`, `expect` are globals here (vitest `globals: true` / Jest default),
// so this exact file runs in both typescript-vitest and typescript-jest.
import { fizzBuzz, sequence } from '../src/fizzbuzz';

describe('fizzBuzz', () => {
  it('returns the number as text', () => {
    expect(fizzBuzz(1)).toBe('1');
  });

  // Table-driven test (like [Theory] + [InlineData]). `$n` / `$expected` go in the name.
  it.each([
    { n: 3, expected: 'Fizz' },
    { n: 5, expected: 'Buzz' },
    { n: 15, expected: 'FizzBuzz' },
    { n: 30, expected: 'FizzBuzz' },
  ])('fizzBuzz($n) is $expected', ({ n, expected }) => {
    expect(fizzBuzz(n)).toBe(expected);
  });

  // Array rows + printf-style placeholders.
  it.each([
    [0, 'zero'],
    [-3, 'negative'],
    [1.5, 'fraction'],
  ])('rejects %d (%s)', (n) => {
    expect(() => fizzBuzz(n)).toThrow(RangeError);
  });

  it('produces a sequence', () => {
    expect(sequence(5)).toEqual(['1', '2', 'Fizz', '4', 'Buzz']);
  });
});
