import { render, screen } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { FizzBuzzForm } from '../../src/fizzbuzz/FizzBuzzForm';

describe('<FizzBuzzForm />', () => {
  it('disables the button until a number is typed', async () => {
    const user = userEvent.setup();
    render(<FizzBuzzForm />);

    expect(screen.getByRole('button', { name: 'Convert' })).toBeDisabled();

    await user.type(screen.getByLabelText('Number'), '3');

    expect(screen.getByRole('button', { name: 'Convert' })).toBeEnabled();
  });

  it('shows the result after submitting', async () => {
    const user = userEvent.setup();
    render(<FizzBuzzForm />);

    await user.type(screen.getByLabelText('Number'), '15');
    await user.click(screen.getByRole('button', { name: 'Convert' }));

    // <output> has the implicit ARIA role "status".
    expect(screen.getByRole('status')).toHaveTextContent('FizzBuzz');
  });

  it('submits with the Enter key and notifies the parent', async () => {
    const user = userEvent.setup();
    const onResult = vi.fn(); // mock function: records its calls (jest.fn() in Jest)
    render(<FizzBuzzForm onResult={onResult} />);

    await user.type(screen.getByLabelText('Number'), '5{Enter}');

    expect(onResult).toHaveBeenCalledWith('Buzz');
  });

  it('queryBy* returns null instead of throwing: use it to assert absence', () => {
    render(<FizzBuzzForm />);

    expect(screen.queryByRole('status')).not.toBeInTheDocument();
  });
});
