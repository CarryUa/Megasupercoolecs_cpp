import { useState, useEffect } from "react";
import { api, loadSchema, initValues, serialize, cacheSchema } from "../utils";
import Fields from "./Fields";

export default function ComponentMapField({ value = {}, onChange }) {
  const [types, setTypes] = useState([]);
  const [selected, setSelected] = useState("");
  const [schemas, setSchemas] = useState({});

  useEffect(() => {
    api
      .fetchComponentTypes?.()
      .then((names) => {
        setTypes(names);
        if (names.length) setSelected(names[0]);
      })
      .catch(() => {});
  }, []);

  useEffect(() => {
    const missing = Object.keys(value ?? {}).filter((t) => !schemas[t]);
    if (!missing.length) return;
    missing.forEach((t) => {
      loadSchema(t)
        .then((s) => {
          cacheSchema(t, s);
          setSchemas((prev) => ({ ...prev, [t]: s }));
        })
        .catch(() => {});
    });
  }, [value]);

  const add = () => {
    if (!selected || (value ?? {})[selected]) return;
    loadSchema(selected).then((s) => {
      cacheSchema(selected, s);
      setSchemas((prev) => ({ ...prev, [selected]: s }));
      onChange({ ...(value ?? {}), [selected]: initValues(s) });
    });
  };

  const remove = (t) => {
    const next = { ...(value ?? {}) };
    delete next[t];
    onChange(next);
  };

  return (
    <div className='mt-2 border border-[#45475a] rounded-lg p-2'>
      <div className='flex items-center gap-2'>
        <select
          value={selected}
          onChange={(e) => setSelected(e.target.value)}
          className='flex-1 p-2 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded'
        >
          {types.map((t) => (
            <option key={t} value={t}>
              {t}
            </option>
          ))}
        </select>
        <button
          type='button'
          onClick={add}
          className='px-3 py-1 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded hover:bg-[#45475a]'
        >
          + Add
        </button>
      </div>
      <div className='mt-2 flex flex-col gap-2'>
        {Object.entries(value ?? {}).map(([t, v]) => (
          <div key={t} className='border border-[#45475a] rounded p-2'>
            <div className='flex items-center justify-between'>
              <b>{t}</b>
              <button type='button' onClick={() => remove(t)}>
                −
              </button>
            </div>
            {schemas[t] && (
              <Fields schema={schemas[t]} value={v} onChange={(nv) => onChange({ ...(value ?? {}), [t]: nv })} />
            )}
          </div>
        ))}
      </div>
    </div>
  );
}
