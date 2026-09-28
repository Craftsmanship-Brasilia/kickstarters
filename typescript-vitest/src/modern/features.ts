// A tour of newer TypeScript/JavaScript features (TS 4.9 → 7, ES2022 → ES2024).

// 1) `satisfies` — validate a value against a type but keep the precise inferred type.
type Route = { path: string; auth: boolean };
export const routes = {
  home: { path: '/', auth: false },
  admin: { path: '/admin', auth: true },
} satisfies Record<string, Route>;
// routes.home is still known (with `: Record<string, Route>` it would be `Route | undefined`).

// 2) `const` type parameters (TS 5.0) — infer literal types without `as const` at the call site.
export function tuple<const T extends readonly unknown[]>(...items: T): T {
  return items;
}

// 3) Inferred type predicates (TS 5.5) — `filter` now narrows (string | null)[] to string[].
export function presentNames(names: (string | null)[]): string[] {
  return names.filter((name) => name !== null);
}

// 4) `NoInfer` (TS 5.4) — stop TS from inferring T from a particular argument.
export function pick<T extends string>(options: T[], fallback: NoInfer<T>): T {
  return options[0] ?? fallback;
}

// 5) `using` + Symbol.dispose (TS 5.2, explicit resource management).
//    The `Disposable` type comes from @types/node here (or add "esnext.disposable" to "lib").
//    the resource is disposed automatically when the block ends, like C# `using`.
export class Connection implements Disposable {
  static readonly log: string[] = [];
  constructor(readonly name: string) {
    Connection.log.push(`open ${name}`);
  }
  [Symbol.dispose](): void {
    Connection.log.push(`close ${this.name}`);
  }
}

export function useConnection(): string {
  using connection = new Connection('db');
  return `using ${connection.name}`;
} // <- connection[Symbol.dispose]() runs here

// 6) Modern standard library (ES2023/ES2024, Node 22).
export const groupByLength = (words: string[]) => Object.groupBy(words, (word) => word.length);
export const sortedCopy = (numbers: readonly number[]) => numbers.toSorted((a, b) => a - b);
export const lastItem = <T>(items: readonly T[]) => items.at(-1);

// 7) Optional chaining (?.), nullish coalescing (??) and ??= (ES2020/2021).
type Settings = { theme?: { color?: string } };
export function colorOf(settings?: Settings): string {
  return settings?.theme?.color ?? 'default';
}

// 8) Branded types — make structurally identical types incompatible.
export type UserId = string & { readonly __brand: 'UserId' };
export const userId = (raw: string) => raw as UserId;
