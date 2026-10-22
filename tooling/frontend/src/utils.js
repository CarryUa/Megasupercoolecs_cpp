const intTypes = new Set([
  "int",
  "short",
  "long",
  "long long",
  "unsigned",
  "unsigned int",
  "unsigned short",
  "unsigned long",
  "unsigned long long",
  "int8_t",
  "int16_t",
  "int32_t",
  "int64_t",
  "uint8_t",
  "uint16_t",
  "uint32_t",
  "uint64_t",
  "size_t",
  "char",
  "signed char",
  "unsigned char",
]);
const floatTypes = new Set(["float", "double", "long double"]);
const boolTypes = new Set(["bool"]);
const stringTypes = new Set(["string", "std::string", "std::string_view", "string_view"]);

export const isVectorType = (cppType) => /^\s*std::vector\s*<.*>\s*$/.test(String(cppType));

export const parseVectorElemType = (cppType) => {
  const t = String(cppType).trim();
  const start = t.indexOf("<");
  const end = t.lastIndexOf(">");
  if (start === -1 || end === -1 || end <= start) return "std::string";
  return t.slice(start + 1, end).trim();
};

export const inputTypeFor = (cppType) => {
  const lower = String(cppType).trim().toLowerCase();
  if (boolTypes.has(lower)) return { type: "checkbox" };
  if (intTypes.has(lower)) return { type: "number", step: "1" };
  if (floatTypes.has(lower)) return { type: "number", step: "any", inputMode: "decimal" };
  return { type: "text" };
};

export const serializeValue = (cppType, raw) => {
  const lower = String(cppType).trim().toLowerCase();
  if (boolTypes.has(lower)) return !!raw;
  if (intTypes.has(lower)) {
    const n = parseInt(raw, 10);
    return Number.isFinite(n) ? n : 0;
  }
  if (floatTypes.has(lower)) {
    const n = parseFloat(raw);
    return Number.isFinite(n) ? n : 0;
  }
  return raw ?? "";
};

export const api = {
  fetchTypes: async () => {
    const res = await fetch("/prototype/whois");
    if (!res.ok) throw new Error(`GET /prototype/whois -> ${res.status}`);
    const data = await res.json();
    return [...(data.typenames ?? [])].sort((a, b) => a.localeCompare(b));
  },
  fetchFields: async (typename) => {
    const res = await fetch("/type/fields", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ type: typename }),
    });
    const data = await res.json();
    const fields = data.fields ?? [];
    if (Array.isArray(fields)) {
      return fields.map((f) => {
        if (Array.isArray(f))
          return { name: f[0], type: f[1]?.first ?? f[1], is_class: f[1]?.second ?? false, is_enum: false };
        if (f && typeof f === "object" && ("name" in f || "type" in f))
          return { name: f.name, type: f.type, is_class: !!f.is_class, is_enum: !!f.is_enum };
        return {
          name: f?.key ?? f?.name,
          type: f?.value ?? f?.type,
          is_class: f?.is_class ?? false,
          is_enum: f?.is_enum ?? false,
        };
      });
    }
    return Object.entries(fields).map(([name, v]) =>
      v && typeof v === "object" && ("first" in v || "second" in v)
        ? { name, type: v.first, is_class: !!v.second, is_enum: false }
        : { name, type: v, is_class: false, is_enum: false },
    );
  },
  fetchPrototypeIds: async () => {
    const res = await fetch("/prototype/ids");
    if (!res.ok) throw new Error(`GET /prototype/ids -> ${res.status}`);
    const data = await res.json();
    return [...(data.ids ?? [])];
  },
  fetchPrototypeFields: async (id) => {
    const res = await fetch("/prototype/fields", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ id }),
    });
    if (!res.ok) throw new Error(`POST /prototype/fields -> ${res.status}`);
    return res.json();
  },
  fetchEntityIds: async () => {
    const res = await fetch("/entity/ids");
    if (!res.ok) throw new Error(`GET /entity/ids -> ${res.status}`);
    const data = await res.json();
    return [...(data.ids ?? [])];
  },
  fetchEntityFields: async (id) => {
    const res = await fetch("/entity/fields", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ id }),
    });
    if (!res.ok) throw new Error(`POST /entity/fields -> ${res.status}`);
    return res.json();
  },
  fetchComponentTypes: async () => {
    const res = await fetch("/component/whois");
    if (!res.ok) throw new Error(`GET /types -> ${res.status}`);
    const data = await res.json();
    return [...(data.types ?? [])].sort((a, b) => a.localeCompare(b));
  },
  fetchAllTypes: async () => {
    const res = await fetch("/types");
    if (!res.ok) throw new Error(`GET /types -> ${res.status}`);
    const data = await res.json();
    return [...(data.typenames ?? [])].sort((a, b) => a.localeCompare(b));
  },
  submit: async (action, data) => {
    const res = await fetch(action, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data),
    });
    return res;
  },
};

export async function loadSchema(type, seen = new Set()) {
  if (seen.has(type)) return { type, fields: [] };
  seen.add(type);
  const fields = await api.fetchFields(type);
  const children = await Promise.all(
    fields.map(async (f) => {
      const isVec = isVectorType(f.type);
      const isPlainClass = f.is_class && f.type !== "std::string" && !isVec;
      const node = { ...f, fields: [] };
      if (isVec) {
        const elemType = parseVectorElemType(f.type);
        try {
          const elemFields = await api.fetchFields(elemType);
          if (elemFields.length > 0 && elemType !== "std::string" && !isVectorType(elemType)) {
            node.elemSchema = await loadSchema(elemType, seen);
          }
        } catch {}
      }
      if (isPlainClass) {
        node.fields = (await loadSchema(f.type, seen)).fields;
      }
      return node;
    }),
  );
  seen.delete(type);
  return { type, fields: children };
}

export function initValuesFromData(schema, data) {
  const obj = {};
  for (const f of schema.fields ?? []) {
    const isVec = isVectorType(f.type);
    const isCompMap = f.type === "TypeComponentMap";
    const isPlainClass = f.is_class && f.type !== "std::string" && !isVec && !isCompMap;
    if (isVec) obj[f.name] = Array.isArray(data?.[f.name]) ? data[f.name] : [];
    else if (isCompMap) obj[f.name] = data?.[f.name] ?? {};
    else if (isPlainClass) obj[f.name] = initValuesFromData(f, data?.[f.name] ?? {});
    else obj[f.name] = data?.[f.name] ?? (f.type === "bool" ? false : "");
  }
  return obj;
}

export function initValues(schema) {
  const obj = {};
  for (const f of schema.fields ?? []) {
    const isVec = isVectorType(f.type);
    const isPlainClass = f.is_class && f.type !== "std::string" && !isVec;
    if (isVec) obj[f.name] = [];
    else if (isPlainClass) obj[f.name] = initValues(f);
    else obj[f.name] = f.type === "bool" ? false : "";
  }
  return obj;
}

const schemaCache = {};
export function cacheSchema(type, schema) {
  schemaCache[type] = schema;
}

export function serialize(schema, values) {
  const out = {};
  for (const f of schema.fields ?? []) {
    const v = values?.[f.name];
    const isVec = isVectorType(f.type);
    const isCompMap = f.type === "TypeComponentMap";
    const isPlainClass = f.is_class && f.type !== "std::string" && !isVec && !isCompMap;
    if (isVec) {
      const elemType = parseVectorElemType(f.type);
      out[f.name] = (v ?? []).map((el) => (f.elemSchema ? serialize(f.elemSchema, el) : serializeValue(elemType, el)));
    } else if (isCompMap) {
      const obj = {};
      for (const [key, val] of Object.entries(v ?? {})) {
        const s = schemaCache[key];
        obj[key] = s ? serialize(s, val ?? {}) : (val ?? {});
      }
      out[f.name] = obj;
    } else if (isPlainClass) {
      out[f.name] = serialize(f, v ?? {});
    } else {
      out[f.name] = serializeValue(f.type, v);
    }
  }
  return out;
}
