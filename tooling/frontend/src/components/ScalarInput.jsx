import { inputTypeFor } from "../utils";

export default function ScalarInput({ cppType, value, onChange }) {
  const { type, step, inputMode } = inputTypeFor(cppType);
  if (type === "checkbox") {
    return (
      <input
        type="checkbox"
        checked={!!value}
        onChange={(e) => onChange(e.target.checked)}
        className="accent-[#aa3bff]"
      />
    );
  }
  return (
    <input
      type={type}
      step={step}
      inputMode={inputMode}
      value={value ?? ""}
      onChange={(e) => onChange(e.target.value)}
      className="bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded px-2 py-1"
    />
  );
}
