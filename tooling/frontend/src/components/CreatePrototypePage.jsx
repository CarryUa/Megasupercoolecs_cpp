import { useEffect, useState } from "react";
import { api, loadSchema, initValues, serialize } from "../utils";
import Fields from "./Fields";

export default function CreatePrototypePage() {
  const [types, setTypes] = useState([]);
  const [selected, setSelected] = useState("");
  const [status, setStatus] = useState("Loading…");
  const [schema, setSchema] = useState(null);
  const [values, setValues] = useState({});
  const [errors, setErrors] = useState([]);

  useEffect(() => {
    api
      .fetchTypes()
      .then((names) => {
        setTypes(names);
        if (names.length) setSelected(names[0]);
        setStatus(names.length ? `${names.length} prototype(s) loaded.` : "No prototypes registered.");
      })
      .catch((e) => setStatus(`Failed to load: ${e.message}`));
  }, []);

  useEffect(() => {
    if (!selected) {
      setSchema(null);
      setValues({});
      return;
    }
    let cancelled = false;
    loadSchema(selected)
      .then((s) => {
        if (cancelled) return;
        setSchema(s);
        setValues(initValues(s));
      })
      .catch((e) => setStatus(`Failed to load fields: ${e.message}`));
    return () => {
      cancelled = true;
    };
  }, [selected]);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setErrors([]);
    if (!schema) return;
    const data = { type: selected, fields: serialize(schema, values) };
    try {
      const res = await api.submit("/prototype/create", data);
      const body = await res.json().catch(() => ({}));
      const errs = Array.isArray(body?.errors) ? body.errors : [];
      if (!res.ok || errs.length) {
        setErrors(errs.length ? errs : [body?.message ?? `Submit failed: ${res.status}`]);
        setStatus("Submit failed.");
      } else {
        setStatus("Submitted successfully.");
      }
    } catch (err) {
      setErrors([err.message]);
      setStatus(`Submit failed: ${err.message}`);
    }
  };

  return (
    <section className="space-y-2">
      <p className={`min-h-[1.5em] ${status.startsWith("Submit failed") || status.startsWith("Failed") ? "text-[#f38ba8]" : "text-[#a6adc8]"}`}>
        {status}
      </p>
      <form onSubmit={handleSubmit} className="space-y-2">
        <label htmlFor="typename-select" className="block mb-1">
          Typename:
        </label>
        <select
          id="typename-select"
          value={selected}
          onChange={(e) => setSelected(e.target.value)}
          disabled={types.length === 0}
          className="w-full p-2 text-base bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded"
        >
          {types.length === 0 && <option value="">Loading…</option>}
          {types.map((t) => (
            <option key={t} value={t}>
              {t}
            </option>
          ))}
        </select>

        {schema && <Fields schema={schema} value={values} onChange={setValues} />}

        {errors.length > 0 && (
          <ul className="text-[#f38ba8] my-2 pl-5 empty:hidden">
            {errors.map((msg, i) => (
              <li key={i}>{String(msg)}</li>
            ))}
          </ul>
        )}

        <button
          type="submit"
          className="mt-2 px-3 py-1 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded hover:bg-[#45475a]"
        >
          Submit
        </button>
      </form>
    </section>
  );
}
