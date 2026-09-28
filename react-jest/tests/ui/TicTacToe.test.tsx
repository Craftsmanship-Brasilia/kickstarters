// UI (interface) tests: render the component into jsdom and use it like a user would.
//  - `render` mounts the component
//  - `screen` queries the document, preferably by ROLE and accessible NAME
//  - `userEvent` simulates real interactions (click, type, tab...)
//  - matchers like toBeDisabled/toHaveTextContent come from @testing-library/jest-dom
import { render, screen, within } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { TicTacToe } from '../../src/tictactoe/TicTacToe';

// Small helper: set up a user and render. `userEvent.setup()` before render is the recommended pattern.
function renderGame() {
  const user = userEvent.setup();
  render(<TicTacToe />);
  const cell = (row: number, column: number) =>
    screen.getByRole('button', { name: `row ${row} column ${column}` });
  return { user, cell };
}

describe('<TicTacToe />', () => {
  it('starts with an empty board and X to play', () => {
    renderGame();

    const board = screen.getByRole('group', { name: 'board' });
    // `within` scopes queries to part of the page.
    expect(within(board).getAllByRole('button')).toHaveLength(9);
    expect(screen.getByRole('status')).toHaveTextContent('X to play');
  });

  it('marks the clicked cell and passes the turn', async () => {
    const { user, cell } = renderGame();

    // userEvent calls are async: always `await` them.
    await user.click(cell(1, 1));

    expect(cell(1, 1)).toHaveTextContent('X');
    expect(cell(1, 1)).toBeDisabled();
    expect(screen.getByRole('status')).toHaveTextContent('O to play');
  });

  it('announces the winner and locks the board', async () => {
    const { user, cell } = renderGame();

    for (const [row, column] of [
      [1, 1],
      [2, 1],
      [1, 2],
      [2, 2],
      [1, 3],
    ]) {
      await user.click(cell(row, column));
    }

    expect(screen.getByRole('status')).toHaveTextContent('X wins');
    expect(cell(3, 3)).toBeDisabled();
  });

  it('starts a new game', async () => {
    const { user, cell } = renderGame();
    await user.click(cell(2, 2));

    await user.click(screen.getByRole('button', { name: 'New game' }));

    expect(cell(2, 2)).toBeEmptyDOMElement();
    expect(screen.getByRole('status')).toHaveTextContent('X to play');
  });
});
