# Cryptography Concepts: Substitution and Transpositional Ciphers with Web Dashboard

## Overview

This project presents an implementation of **classical cryptographic algorithms**, split into two primary categories: **Substitution** and **Transpositional Ciphers**. Each algorithm is written in **C** , and a user-friendly **React-based Web Dashboard** is developed for demonstrating and interacting with these ciphers visually.

---

## Implemented Algorithms

### Substitution Ciphers
- 1. **Caesar Cipher** - `Caesar.c`
- 2. **Atbash Cipher** - `Atbash.c`
- 3. **August Cipher** - `August.c`
- 4. **Affine Cipher** - `Affine.c`
- 5. **Vigenère Cipher** - `Vigenere.c`
- 6. **Gronsfeld Cipher** - `Gronsfeld.c`
- 7. **Beaufort Cipher** - `Beaufort.c`
- 8. **Autokey Cipher / Running Key Cipher** - `Autokey.c`
- 9. **N-Gram Operations** - `ngram.c`
- 10. **Hill Cipher** - `Hill.c`

### Transpositional Ciphers
- 11. **Rail Fence Cipher** - `Rail.c`
- 12. **Route Cipher** - `Route.c`
- 13. **Myszkowski Cipher** - `Myszkowiski.c`

---

## Web Dashboard


### Key Files

- `App.jsx` – Main React component that manages routing and rendering of the cipher components.
- `CryptoDashboard.css` – Styling for the web UI components.
- `main.jsx` – React application entry point.

### Technologies Used
- ReactJS
- Vite (for bundling and dev server)
- CSS3

---

## How to Run

### Prerequisites
- Node.js and npm installed

### Steps to Run the Web Dashboard

```bash
cd Web/Cryptography
npm install
npm run dev

