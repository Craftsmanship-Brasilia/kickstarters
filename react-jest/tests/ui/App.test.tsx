import { render, screen } from '@testing-library/react';
import { App } from '../../src/App';

describe('<App />', () => {
  it('renders both katas', () => {
    render(<App />);

    expect(screen.getByRole('heading', { level: 1, name: 'Kata' })).toBeInTheDocument();
    expect(screen.getByRole('form', { name: 'FizzBuzz' })).toBeInTheDocument();
    expect(screen.getByRole('heading', { name: 'Tic Tac Toe' })).toBeInTheDocument();
  });
});
