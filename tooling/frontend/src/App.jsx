import { useState } from "react";
import CreatePrototypePage from "./components/CreatePrototypePage";
import EditPrototype from "./components/EditPrototype";
import CreateEntityPage from "./components/CreateEntityPage";
import EditEntity from "./components/EditEntity";

const SECTIONS = [
  { id: "create_prototype", label: "Create Prototype" },
  { id: "edit_prototype", label: "Edit Prototype" },
  { id: "create_entity", label: "Create Entity" },
  { id: "edit_entity", label: "Edit Entity" },
];

export default function App() {
  const [section, setSection] = useState(SECTIONS[0].id);

  return (
    <main className="min-w-[480px] mx-auto">
      <nav className="flex gap-2 mb-4 border-b border-[#45475a] pb-2">
        {SECTIONS.map((s) => (
          <button
            key={s.id}
            type="button"
            onClick={() => setSection(s.id)}
            className={`px-3 py-1 rounded border ${
              section === s.id
                ? "bg-[#45475a] text-[#cdd6f4] border-[#45475a]"
                : "bg-[#313244] text-[#cdd6f4] border-[#45475a] hover:bg-[#3b3d52]"
            }`}
          >
            {s.label}
          </button>
        ))}
      </nav>

      {section === "create_prototype" && <CreatePrototypePage />}
      {section === "edit_prototype" && <EditPrototype />}
      {section === "create_entity" && <CreateEntityPage />}
      {section === "edit_entity" && <EditEntity />}
      {section !== "create_prototype" && section !== "edit_prototype" && section !== "create_entity" && section !== "edit_entity" && (
        <p className="text-[#a6adc8]">This section is a placeholder.</p>
      )}
    </main>
  );
}
