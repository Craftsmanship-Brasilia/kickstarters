// Every way of importing, side by side.

// 1) Default import — you choose the local name.
import fizzBuzzDefault from '../src/fizzbuzz/fizzbuzz';

// 2) Named imports through the barrel (../src/fizzbuzz resolves to ../src/fizzbuzz/index.ts).
import { fizzBuzz, rules } from '../src/fizzbuzz';

// 3) Named import with a local alias.
import { sequence as fizzBuzzSequence } from '../src/fizzbuzz';

// 4) Namespace import — the whole module as one object.
import * as FizzBuzzModule from '../src/fizzbuzz';

// 5) Type-only import — erased from the output, can only be used as a type.
import type { Rule } from '../src/fizzbuzz';

// 6) Mixed: a value and an inline type-only import in one statement.
import { Game, type GameStatus } from '../src/tictactoe';

describe('modules', () => {
  it('default and named imports point to the same function', () => {
    expect(fizzBuzzDefault).toBe(fizzBuzz);
  });

  it('aliases and namespaces are just other names for the same exports', () => {
    expect(fizzBuzzSequence).toBe(FizzBuzzModule.sequence);
    expect(Object.keys(FizzBuzzModule).sort()).toEqual([
      'fizzBuzz',
      'rules',
      'sequence',
      'wordsFor',
    ]);
  });

  it('types can be used for annotations, but not as values', () => {
    const bang: Rule = { divisor: 7, word: 'Bang' };

    expect(FizzBuzzModule.wordsFor(21, [...rules, bang])).toBe('FizzBang');
  });

  it('inline type imports', () => {
    const status: GameStatus = Game.start().status;

    expect(status.kind).toBe('in-progress');
  });

  // 7) Dynamic import — loads the module lazily and returns a Promise of the namespace.
  it('dynamic import', async () => {
    const { fizzBuzz: lazyFizzBuzz } = await import('../src/fizzbuzz');

    expect(lazyFizzBuzz(3)).toBe('Fizz');
  });
});
