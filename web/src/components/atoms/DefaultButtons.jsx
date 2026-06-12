export default function DefaultButton({ onClick, className, Text: text }) {
  return (
    <button onClick={onClick} className={className}>
      {text}
    </button>
  );
}
