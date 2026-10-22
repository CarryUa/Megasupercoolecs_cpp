import { useEffect, useState } from "react";
import { api, loadSchema, isVectorType, serialize, initValuesFromData } from "../utils";
import Fields from "./Fields";

export default function EditPrototype() {
  const [ids, setIds] = useState([]);
  const [preferedId, setPreferedId] = useState("");
  const [status, setStatus] = useState("Loading ids…");
  const [error, setError] = useState("");

  const [loadedId, setLoadedId] = useState("");
  const [typeName, setTypeName] = useState("");
  const [schema, setSchema] = useState(null);
  const [values, setValues] = useState({});
  const [errors, setErrors] = useState([]);

  useEffect(() => {
    api
      .fetchPrototypeIds()
      .then((list) => {
        setIds(list);
        if (list.length) setPreferedId(list[0]);
        setStatus(list.length ? `${list.length} prototype(s) found.` : "No prototypes found.");
      })
      .catch((e) => setStatus(`Failed to load ids: ${e.message}`));
  }, []);

  const load = async (id) => {
    const target = id || preferedId;
    if (!target) {
      setError("No prototype id selected.");
      return;
    }
    setError("");
    setErrors([]);
    setStatus("Loading…");
    setSchema(null);
    setValues({});
    try {
      const body = await api.fetchPrototypeFields(target);
      setLoadedId(body.id);
      setTypeName(body.type);
      const s = await loadSchema(body.type);
      setSchema(s);
      setValues(initValuesFromData(s, body.fields ?? {}));
      setStatus(`Loaded ${target}.`);
    } catch (e) {
      setError(e.message);
      setStatus("Failed to load.");
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setErrors([]);
    if (!schema || !loadedId) return;
    const data = { id: loadedId, type: typeName, fields: serialize(schema, values) };
    try {
      const res = await api.submit("/prototype/edit", data);
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
    <section className="space-y-3">
      <p className={`min-h-[1.5em] ${error || errors.length ? "text-[#f38ba8]" : "text-[#a6adc8]"}`}>
        {error || status}
      </p>

      <div className="flex items-center gap-2">
        <label htmlFor="prototype-id-select" className="block">
          Prototype id:
        </label>
        <select
          id="prototype-id-select"
          value={preferedId}
          onChange={(e) => setPreferedId(e.target.value)}
          className="flex-1 p-2 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded"
          disabled={ids.length === 0}
        >
          {ids.length === 0 && <option value="">No ids</option>}
          {ids.map((id) => (
            <option key={id} value={id}>
              {id}
            </option>
          ))}
        </select>
        <button
          type="button"
          onClick={() => load(preferedId)}
          className="px-3 py-1 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded hover:bg-[#45475a]"
        >
          Load
        </button>
      </div>

      {schema && (
        <form onSubmit={handleSubmit} className="space-y-2">
          <p className="text-[#a6adc8]">
            id: <b>{loadedId}</b> &nbsp; type: <b>{typeName}</b>
          </p>
          <Fields schema={schema} value={values} onChange={setValues} />

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
            Save
          </button>
        </form>
      )}
    </section>
  );
}


