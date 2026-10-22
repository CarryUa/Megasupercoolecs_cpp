import { useState, useEffect } from "react";
import { isVectorType, parseVectorElemType, initValues, api, loadSchema } from "../utils";
import ScalarInput from "./ScalarInput";
import Fields from "./Fields";

export default function VectorField({ field, value = [], onChange }) {
  const elemType = parseVectorElemType(field.type);
  const [elemIsVector, setElemIsVector] = useState(isVectorType(elemType));
  const [elemSchema, setElemSchema] = useState(null);

  useEffect(() => {
    if (isVectorType(elemType)) {
      setElemIsVector(true);
      return;
    }
    let cancelled = false;
    api.fetchFields(elemType).then((f) => {
      if (cancelled) return;
      if (f.length > 0 && elemType !== "std::string") {
        loadSchema(elemType).then((s) => {
          if (!cancelled) setElemSchema(s);
        });
      }
    });
    return () => {
      cancelled = true;
    };
  }, [elemType]);

  const size = value.length;
  const setSize = (n) => {
    n = Math.max(0, Math.floor(Number(n) || 0));
    const next = [...value];
    while (next.length < n) next.push(elemSchema ? initValues(elemSchema) : elemType === "bool" ? false : "");
    while (next.length > n) next.pop();
    onChange(next);
  };

  const isElemClass = !!elemSchema;
  const defaultElem = isElemClass ? initValues(elemSchema) : elemType === "bool" ? false : "";

  return (
    <div className="mt-2 border border-[#45475a] rounded-lg p-2">
      <div className="flex items-center gap-2">
        <span>Size</span>
        <input
          type="number"
          min="0"
          step="1"
          value={size}
          onChange={(e) => setSize(e.target.value)}
          className="w-16 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded px-2"
        />
        <button
          type="button"
          className="px-2 py-0.5 bg-[#313244] text-[#cdd6f4] border border-[#45475a] rounded hover:bg-[#45475a]"
          onClick={() => onChange([...value, defaultElem])}
        >
          + Add
        </button>
      </div>
      <div className="mt-2 flex flex-col gap-1">
        {value.map((v, i) => (
          <div className="flex items-center gap-2" key={i}>
            <span className="vector-list-index">Element {i}</span>
            {isVectorType(elemType) ? (
              <VectorField
                field={{ type: elemType, name: field.name }}
                value={v}
                onChange={(nv) => {
                  const next = [...value];
                  next[i] = nv;
                  onChange(next);
                }}
              />
            ) : isElemClass ? (
              <Fields
                schema={elemSchema}
                value={v}
                onChange={(nv) => {
                  const next = [...value];
                  next[i] = nv;
                  onChange(next);
                }}
              />
            ) : (
              <ScalarInput
                cppType={elemType}
                value={v}
                onChange={(nv) => {
                  const next = [...value];
                  next[i] = nv;
                  onChange(next);
                }}
              />
            )}
            <button
              type="button"
              onClick={() => onChange(value.filter((_, idx) => idx !== i))}
            >
              −
            </button>
          </div>
        ))}
      </div>
    </div>
  );
}
