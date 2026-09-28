import { useState, type FormEvent } from 'react';
import { fizzBuzz } from './fizzbuzz';

// Props type: an optional callback lets tests (and parents) observe what happened.
type FizzBuzzFormProps = {
  onResult?: (result: string) => void;
};

export function FizzBuzzForm({ onResult }: FizzBuzzFormProps) {
  const [value, setValue] = useState('');
  const [result, setResult] = useState<string | null>(null);

  function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault(); // stop the browser from reloading the page
    const answer = fizzBuzz(Number(value));
    setResult(answer);
    onResult?.(answer); // optional chaining call: only if the prop was passed
  }

  return (
    <form aria-label="FizzBuzz" onSubmit={handleSubmit}>
      {/* The <label> gives the input its accessible name: getByLabelText('Number') */}
      <label>
        Number
        <input type="number" value={value} onChange={(event) => setValue(event.target.value)} />
      </label>
      <button type="submit" disabled={value === ''}>
        Convert
      </button>
      {/* Conditional rendering: <output> only exists after the first submit */}
      {result !== null && <output>{result}</output>}
    </form>
  );
}
