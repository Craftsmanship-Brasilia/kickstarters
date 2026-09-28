// A module is just a file. Anything not exported stays private to this file.

// `export type` — a type-only export: it disappears completely from the JavaScript output.
export type Rule = {
  readonly divisor: number;
  readonly word: string;
};

// `as const` makes the array readonly and its values literal types ('Fizz', not string).
// `satisfies` checks the value against a type WITHOUT widening it (TS 4.9).
export const RULES = [
  { divisor: 3, word: 'Fizz' },
  { divisor: 5, word: 'Buzz' },
] as const satisfies readonly Rule[];

// Not exported: private helper of this module.
const isMultipleOf = (divisor: number) => (n: number) => n % divisor === 0;

// Named function export.
export function wordsFor(n: number, rules: readonly Rule[] = RULES): string {
  return rules
    .filter((rule) => isMultipleOf(rule.divisor)(n))
    .map((rule) => rule.word)
    .join('');
}
