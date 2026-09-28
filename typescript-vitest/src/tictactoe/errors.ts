import type { Position } from './types';

// Custom error class. `override` is required when redefining a member of the base class
// (with "noImplicitOverride"), and documents intent anyway.
export class PositionTakenError extends Error {
  override readonly name = 'PositionTakenError';

  constructor(readonly position: Position) {
    super(`Position ${position.row},${position.column} is already taken`);
  }
}
