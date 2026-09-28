import {
  Connection,
  colorOf,
  groupByLength,
  lastItem,
  pick,
  presentNames,
  routes,
  sortedCopy,
  tuple,
  useConnection,
  userId,
  type UserId,
} from '../src/modern/features';

describe('modern TypeScript', () => {
  it('satisfies keeps the precise type', () => {
    expect(routes.admin.auth).toBe(true);
  });

  it('const type parameters infer literal tuples', () => {
    const t = tuple('a', 1); // type: readonly ['a', 1]

    expect(t).toEqual(['a', 1]);
  });

  it('filter narrows with inferred type predicates', () => {
    expect(presentNames(['ada', null, 'bob'])).toEqual(['ada', 'bob']);
  });

  it('NoInfer', () => {
    expect(pick(['small', 'large'], 'small')).toBe('small');
    // pick(['small', 'large'], 'medium') would be a compile error
  });

  it('using disposes resources at the end of the scope', () => {
    Connection.log.length = 0;

    expect(useConnection()).toBe('using db');
    expect(Connection.log).toEqual(['open db', 'close db']);
  });

  it('modern standard library', () => {
    expect(groupByLength(['a', 'bb', 'cc'])).toEqual({ 1: ['a'], 2: ['bb', 'cc'] });
    expect(sortedCopy([3, 1, 2])).toEqual([1, 2, 3]);
    expect(lastItem([1, 2, 3])).toBe(3);
  });

  it('optional chaining and nullish coalescing', () => {
    expect(colorOf()).toBe('default');
    expect(colorOf({ theme: { color: 'red' } })).toBe('red');
  });

  it('branded types', () => {
    const id: UserId = userId('42');
    // const wrong: UserId = '42'; // compile error: plain string is not a UserId

    expect(id).toBe('42');
  });
});
