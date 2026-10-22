import { isVectorType } from "../utils";
import ScalarInput from "./ScalarInput";
import VectorField from "./VectorField";
import ComponentMapField from "./ComponentMapField";

export default function Fields({ schema, value = {}, onChange }) {
  return (
    <div>
      {(schema.fields ?? []).map((f) => {
        const isVec = isVectorType(f.type);
        const isCompMap = f.type === "TypeComponentMap";
        const isPlainClass = f.is_class && f.type !== "std::string" && !isVec && !isCompMap;
        return (
          <p className="bg-[#26263a] p-2 rounded-2xl" key={f.name}>
            <span>
              {f.name}: <b>{f.type}</b>{" "}
            </span>
            {isVec ? (
              <VectorField
                field={f}
                value={value?.[f.name] ?? []}
                onChange={(v) => onChange({ ...value, [f.name]: v })}
              />
            ) : isCompMap ? (
              <ComponentMapField
                value={value?.[f.name] ?? {}}
                onChange={(v) => onChange({ ...value, [f.name]: v })}
              />
            ) : isPlainClass ? (
              <div className="ml-6 border-l-2 border-[#45475a] pl-3">
                <Fields
                  schema={f}
                  value={value?.[f.name] ?? {}}
                  onChange={(v) => onChange({ ...value, [f.name]: v })}
                />
              </div>
            ) : (
              <ScalarInput
                cppType={f.type}
                value={value?.[f.name]}
                onChange={(v) => onChange({ ...value, [f.name]: v })}
              />
            )}
          </p>
        );
      })}
    </div>
  );
}
