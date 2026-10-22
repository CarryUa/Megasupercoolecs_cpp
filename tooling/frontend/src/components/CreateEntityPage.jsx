import { useEffect, useState } from "react";
import { api, loadSchema, initValues, serialize } from "../utils";
import Fields from "./Fields";

export default function CreateEntityPage() {
  const [status, setStatus] = useState("Ready.");
  const [schema, setSchema] = useState(null);
  const [values, setValues] = useState({});
  const [errors, setErrors] = useState([]);

  useEffect(() => {
    loadSchema("msce::Entity")
      .then((s) => {
        setSchema(s);
        setValues(initValues(s));
      })
      .catch((e) => setStatus(`Failed to load fields: ${e.message}`));
  }, []);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setErrors([]);
    if (!schema) return;
    const data = { type: "msce::Entity", fields: serialize(schema, values) };
    try {
      const res = await api.submit("/entity/create", data);
      const body = await res.json().catch(() => ({}));
      const errs = Array.isArray(body?.errors) ? body.errors : [];
      if (!res.ok || errs.length) {
        setErrors(errs.length ? errs : [body?.message ?? `Submit failed: ${res.status}`]);
        setStatus("Submit failed.");
      } else {
        setStatus(`Created entity ${body?.id ?? ""}.`);
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
        {schema && <Fields schema={schema} value={values} onChange={setValues} />}
        {errors.length > 0 && (
          <ul className="text-[#f38ba8] my-2 pl-5 empty:hidden">
            {errors.map((msg, i) => (
              <li key={i}>{String(msg)}</li>
            ))}
          </ul>
        )}
        <button type="submit" className="mt-2 px-3 py-1 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded hover:bg-[#45475a]">
          Submit
        </button>
      </form>
    </section>
  );
}