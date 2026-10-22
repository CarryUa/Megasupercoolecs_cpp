import react from '@vitejs/plugin-react'
import tailwindcss from '@tailwindcss/vite'
import { defineConfig } from 'vite'

export default defineConfig({
  plugins: [react(), tailwindcss()],
  server: {
    proxy: {
      '/prototype': 'http://127.0.0.1:8080',
      '/type': 'http://127.0.0.1:8080',
      '/types': 'http://127.0.0.1:8080',
      '/log': 'http://127.0.0.1:8080',
    },
  },
})