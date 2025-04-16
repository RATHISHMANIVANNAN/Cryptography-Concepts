import React, { useState } from 'react';
import './CryptoDashboard.css';

const CryptoDashboard = () => {
  const [algorithm, setAlgorithm] = useState('caesar');
  const [operation, setOperation] = useState('encrypt');
  const [inputText, setInputText] = useState('');
  const [outputText, setOutputText] = useState('');

  // Dynamic parameters state
  const [params, setParams] = useState({
    shift: 3,               // Caesar
    a: 5,                   // Affine
    b: 8,                   // Affine
    key: 'KEY',             // Vigenere, Beaufort, Autokey
    keyNum: '123',          // Gronsfeld
    rails: 3,               // Rail fence
    n: 3,                   // Route, ngram
    keywordMysz: 'KEYWORD', // Myszkowski
    matrix: '2,1,3,4'       // Hill cipher (2x2 matrix)
  });
  const [error, setError] = useState('');

  const algorithmDescriptions = {
    caesar: "Shifts each letter by a fixed number of positions in the alphabet.",
    affine: "Uses mathematical function (ax + b) mod 26 to encrypt each letter.",
    atbash: "Reverses the alphabet (A becomes Z, B becomes Y, etc.).",
    vigenere: "Uses a keyword to determine shifting values for each letter.",
    beaufort: "Similar to Vigenere but uses a different formula for encryption.",
    autokey: "Uses the plaintext itself as part of the key after an initial keyword.",
    gronsfeld: "Similar to Vigenere but uses numbers as the key.",
    railfence: "Writes text in a zigzag pattern on 'rails' and reads off by rows.",
    route: "Arranges text in a grid and reads it off following a route.",
    hill: "Uses matrices to encrypt blocks of letters at once.",
    myszkowski: "Uses a keyword to determine column order in a transposition.",
    ngram: "Processes text in groups of n characters at a time.",
    august: "Shifts each letter by its position in the text plus one."
  };

  const handleParamChange = (key, value) => {
    setParams(prevParams => ({
      ...prevParams,
      [key]: value
    }));
  };

  // Helper functions
  const gcd = (a, b) => {
    if (b === 0) return a;
    return gcd(b, a % b);
  };

  const modInverse = (a, m) => {
    a = ((a % m) + m) % m;
    for (let x = 1; x < m; x++) {
      if ((a * x) % m === 1) {
        return x;
      }
    }
    return -1;
  };

  const mod26 = (x) => {
    return ((x % 26) + 26) % 26;
  };

  // Validation
  const validateInput = () => {
    if (!inputText.trim()) {
      setError('Please enter some text to process');
      return false;
    }

    switch(algorithm) {
      case 'affine':
        const a = parseInt(params.a);
        if (gcd(a, 26) !== 1) {
          setError('Parameter "a" must be coprime with 26');
          return false;
        }
        break;
      case 'hill':
        const matrixValues = params.matrix.split(',').map(Number);
        if (matrixValues.length !== 4) {
          setError('Hill cipher requires 4 matrix values (2x2)');
          return false;
        }
        const det = matrixValues[0] * matrixValues[3] - matrixValues[1] * matrixValues[2];
        if (gcd(mod26(det), 26) !== 1) {
          setError('Matrix determinant must be coprime with 26');
          return false;
        }
        break;
    }
    
    setError('');
    return true;
  };

  const processText = () => {
    if (!validateInput()) return;
    
    let result = '';
    
    switch(algorithm) {
      case 'caesar':
        result = caesarCipher(inputText, parseInt(params.shift), operation === 'encrypt');
        break;
      case 'affine':
        result = affineCipher(inputText, parseInt(params.a), parseInt(params.b), operation === 'encrypt');
        break;
      case 'atbash':
        result = atbashCipher(inputText);
        break;
      case 'vigenere':
        result = vigenereCipher(inputText, params.key, operation === 'encrypt');
        break;
      case 'beaufort':
        result = beaufortCipher(inputText, params.key);
        break;
      case 'autokey':
        result = autokeyCipher(inputText, params.key, operation === 'encrypt');
        break;
      case 'gronsfeld':
        result = gronsfeldCipher(inputText, params.keyNum, operation === 'encrypt');
        break;
      case 'railfence':
        result = railFenceCipher(inputText, parseInt(params.rails), operation === 'encrypt');
        break;
      case 'route':
        result = routeCipher(inputText, parseInt(params.n), operation === 'encrypt');
        break;
      case 'hill':
        const matrixValues = params.matrix.split(',').map(Number);
        const keyMatrix = [
          [matrixValues[0], matrixValues[1]],
          [matrixValues[2], matrixValues[3]]
        ];
        result = hillCipher(inputText, keyMatrix, operation === 'encrypt');
        break;
      case 'myszkowski':
        result = myszkowskiCipher(inputText, params.keywordMysz, operation === 'encrypt');
        break;
      case 'ngram':
        result = ngramCipher(inputText, parseInt(params.n), operation === 'encrypt');
        break;
      case 'august':
        result = augustCipher(inputText, operation === 'encrypt');
        break;
      default:
        result = "Algorithm not implemented";
    }
    
    setOutputText(result);
  };

  const caesarCipher = (text, shift, encrypt) => {
    if (!encrypt) shift = 26 - (shift % 26);
    
    return text.split('').map(char => {
      if (char >= 'a' && char <= 'z') {
        return String.fromCharCode(((char.charCodeAt(0) - 97 + shift) % 26) + 97);
      } else if (char >= 'A' && char <= 'Z') {
        return String.fromCharCode(((char.charCodeAt(0) - 65 + shift) % 26) + 65);
      }
      return char;
    }).join('');
  };

  const affineCipher = (text, a, b, encrypt) => {
    if (encrypt) {
      return text.split('').map(char => {
        if (char >= 'a' && char <= 'z') {
          return String.fromCharCode(((a * (char.charCodeAt(0) - 97) + b) % 26) + 97);
        } else if (char >= 'A' && char <= 'Z') {
          return String.fromCharCode(((a * (char.charCodeAt(0) - 65) + b) % 26) + 65);
        }
        return char;
      }).join('');
    } else {
      const aInv = modInverse(a, 26);
      return text.split('').map(char => {
        if (char >= 'a' && char <= 'z') {
          return String.fromCharCode((aInv * mod26(char.charCodeAt(0) - 97 - b)) % 26 + 97);
        } else if (char >= 'A' && char <= 'Z') {
          return String.fromCharCode((aInv * mod26(char.charCodeAt(0) - 65 - b)) % 26 + 65);
        }
        return char;
      }).join('');
    }
  };

  const atbashCipher = (text) => {
    return text.split('').map(char => {
      if (char >= 'a' && char <= 'z') {
        return String.fromCharCode(219 - char.charCodeAt(0)); // 219 = 'a' + 'z' = 97 + 122
      } else if (char >= 'A' && char <= 'Z') {
        return String.fromCharCode(155 - char.charCodeAt(0)); // 155 = 'A' + 'Z' = 65 + 90
      }
      return char;
    }).join('');
  };

  const vigenereCipher = (text, key, encrypt) => {
    key = key.toUpperCase();
    const keyLength = key.length;
    let result = '';
    let j = 0;

    for (let i = 0; i < text.length; i++) {
      const char = text[i];
      if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
        const isUpper = char >= 'A' && char <= 'Z';
        const base = isUpper ? 65 : 97;
        const charCode = char.charCodeAt(0) - base;
        const keyChar = key[j % keyLength].charCodeAt(0) - 65;
        
        let resultChar;
        if (encrypt) {
          resultChar = (charCode + keyChar) % 26;
        } else {
          resultChar = (charCode - keyChar + 26) % 26;
        }
        
        result += String.fromCharCode(resultChar + base);
        j++;
      } else {
        result += char;
      }
    }
    return result;
  };

  const beaufortCipher = (text, key) => {
    key = key.toUpperCase();
    const keyLength = key.length;
    let result = '';
    let j = 0;

    for (let i = 0; i < text.length; i++) {
      const char = text[i];
      if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
        const isUpper = char >= 'A' && char <= 'Z';
        const base = isUpper ? 65 : 97;
        const charCode = char.toUpperCase().charCodeAt(0) - 65;
        const keyChar = key[j % keyLength].charCodeAt(0) - 65;
        
        const resultChar = (keyChar - charCode + 26) % 26;
        
        result += String.fromCharCode(resultChar + (isUpper ? 65 : 97));
        j++;
      } else {
        result += char;
      }
    }
    return result;
  };

  const autokeyCipher = (text, key, encrypt) => {
    key = key.toUpperCase();
    let cleanText = text.replace(/[^A-Za-z]/g, '').toUpperCase();
    let result = '';
    let j = 0;
    
    if (encrypt) {
      let keyStream = key;
      for (let i = 0; i < text.length; i++) {
        const char = text[i];
        if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
          const isUpper = char >= 'A' && char <= 'Z';
          const base = isUpper ? 65 : 97;
          const charCode = char.toUpperCase().charCodeAt(0) - 65;
          
          const keyChar = keyStream[j].charCodeAt(0) - 65;
          const encryptedChar = (charCode + keyChar) % 26;
          
          result += String.fromCharCode(encryptedChar + (isUpper ? 65 : 97));
          
          if (keyStream.length <= j + cleanText.length) {
            keyStream += char.toUpperCase();
          }
          j++;
        } else {
          result += char;
        }
      }
    } else {
      let keyStream = key;
      for (let i = 0; i < text.length; i++) {
        const char = text[i];
        if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
          const isUpper = char >= 'A' && char <= 'Z';
          const base = isUpper ? 65 : 97;
          const charCode = char.toUpperCase().charCodeAt(0) - 65;
          
          if (j < keyStream.length) {
            const keyChar = keyStream[j].charCodeAt(0) - 65;
            const decryptedChar = (charCode - keyChar + 26) % 26;
            
            const decryptedLetter = String.fromCharCode(decryptedChar + (isUpper ? 65 : 97));
            result += decryptedLetter;
            
            keyStream += String.fromCharCode(decryptedChar + 65);
          }
          j++;
        } else {
          result += char;
        }
      }
    }
    
    return result;
  };

  const gronsfeldCipher = (text, key, encrypt) => {
    const keyLength = key.length;
    let result = '';
    let j = 0;

    for (let i = 0; i < text.length; i++) {
      const char = text[i];
      if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
        const isUpper = char >= 'A' && char <= 'Z';
        const base = isUpper ? 65 : 97;
        const charCode = char.charCodeAt(0) - base;
        const shift = parseInt(key[j % keyLength]);
        
        let resultChar;
        if (encrypt) {
          resultChar = (charCode + shift) % 26;
        } else {
          resultChar = (charCode - shift + 26) % 26;
        }
        
        result += String.fromCharCode(resultChar + base);
        j++;
      } else {
        result += char;
      }
    }
    return result;
  };

  const railFenceCipher = (text, rails, encrypt) => {
    if (rails < 2) return text;
    
    if (encrypt) {
      const fence = Array(rails).fill().map(() => []);
      let rail = 0;
      let direction = 1;

      for (let i = 0; i < text.length; i++) {
        fence[rail].push(text[i]);
        
        if (rail === 0) {
          direction = 1;
        } else if (rail === rails - 1) {
          direction = -1;
        }
        
        rail += direction;
      }
      
      return fence.map(rail => rail.join('')).join('');
    } else {
      const len = text.length;
      const fence = Array(rails).fill().map(() => Array(len).fill(''));
      
      let rail = 0;
      let direction = 1;
      let positions = [];
      
      for (let i = 0; i < len; i++) {
        positions.push(rail);
        
        if (rail === 0) {
          direction = 1;
        } else if (rail === rails - 1) {
          direction = -1;
        }
        
        rail += direction;
      }
      
      let counts = Array(rails).fill(0);
      for (let i = 0; i < len; i++) {
        const rail = positions[i];
        counts[rail]++;
      }
      
      let starts = [0];
      for (let i = 1; i < rails; i++) {
        starts[i] = starts[i-1] + counts[i-1];
      }
      
      let railMap = Array(len).fill(0);
      for (let i = 0; i < len; i++) {
        const rail = positions[i];
        railMap[i] = starts[rail]++;
      }
      
      let result = Array(len).fill('');
      for (let i = 0; i < len; i++) {
        result[i] = text[railMap[i]];
      }
      
      return result.join('');
    }
  };

  const routeCipher = (text, n, encrypt) => {
    let paddedText = text;
    while (paddedText.length < n * n) {
      paddedText += 'X';
    }
    paddedText = paddedText.slice(0, n * n);
    
    if (encrypt) {
      const grid = Array(n).fill().map(() => Array(n).fill(''));
      let index = 0;
      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
          grid[i][j] = paddedText[index++];
        }
      }
      
      let result = '';
      for (let j = 0; j < n; j++) {
        for (let i = 0; i < n; i++) {
          result += grid[i][j];
        }
      }
      return result;
    } else {
      const grid = Array(n).fill().map(() => Array(n).fill(''));
      let index = 0;
      for (let j = 0; j < n; j++) {
        for (let i = 0; i < n; i++) {
          grid[i][j] = paddedText[index++];
        }
      }
      
      let result = '';
      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
          result += grid[i][j];
        }
      }
      return result;
    }
  };

  const hillCipher = (text, keyMatrix, encrypt) => {
    text = text.toUpperCase().replace(/[^A-Z]/g, '');
    if (text.length % 2 !== 0) text += 'X';
    
    let result = '';
    
    if (encrypt) {
      for (let i = 0; i < text.length; i += 2) {
        const p1 = text.charCodeAt(i) - 65;
        const p2 = text.charCodeAt(i + 1) - 65;
        
        const c1 = mod26(keyMatrix[0][0] * p1 + keyMatrix[0][1] * p2);
        const c2 = mod26(keyMatrix[1][0] * p1 + keyMatrix[1][1] * p2);
        
        result += String.fromCharCode(c1 + 65);
        result += String.fromCharCode(c2 + 65);
      }
    } else {
      const det = mod26(keyMatrix[0][0] * keyMatrix[1][1] - keyMatrix[0][1] * keyMatrix[1][0]);
      const detInv = modInverse(det, 26);
      
      const adj = [
        [keyMatrix[1][1], -keyMatrix[0][1]],
        [-keyMatrix[1][0], keyMatrix[0][0]]
      ];
      
      const invMatrix = [
        [mod26(adj[0][0] * detInv), mod26(adj[0][1] * detInv)],
        [mod26(adj[1][0] * detInv), mod26(adj[1][1] * detInv)]
      ];
      
      for (let i = 0; i < text.length; i += 2) {
        const c1 = text.charCodeAt(i) - 65;
        const c2 = text.charCodeAt(i + 1) - 65;
        
        const p1 = mod26(invMatrix[0][0] * c1 + invMatrix[0][1] * c2);
        const p2 = mod26(invMatrix[1][0] * c1 + invMatrix[1][1] * c2);
        
        result += String.fromCharCode(p1 + 65);
        result += String.fromCharCode(p2 + 65);
      }
    }
    
    return result;
  };

  const myszkowskiCipher = (text, keyword, encrypt) => {

    keyword = keyword.toUpperCase();
    const keyChars = keyword.split('');
    
    const sortedKey = [...keyChars].sort();
    
    const numKey = [];
    for (let i = 0; i < keyChars.length; i++) {
      numKey[i] = sortedKey.indexOf(keyChars[i]) + 1;
      sortedKey[sortedKey.indexOf(keyChars[i])] = '';
    }
    
    if (encrypt) {
      const rows = Math.ceil(text.length / keyword.length);
      const grid = Array(rows).fill().map(() => Array(keyword.length).fill(''));
      
      let pos = 0;
      for (let i = 0; i < rows; i++) {
        for (let j = 0; j < keyword.length; j++) {
          if (pos < text.length) {
            grid[i][j] = text[pos++];
          } else {
            grid[i][j] = '';
          }
        }
      }
      
      let result = '';
      const maxNum = Math.max(...numKey);
      
      for (let num = 1; num <= maxNum; num++) {
        for (let j = 0; j < keyword.length; j++) {
          if (numKey[j] === num) {
            for (let i = 0; i < rows; i++) {
              if (grid[i][j] !== '') {
                result += grid[i][j];
              }
            }
          }
        }
      }
      
      return result;
    } else {
      const numChars = text.length;
      const fullRows = Math.floor(numChars / keyword.length);
      const remainderChars = numChars % keyword.length;
      
      const colSizes = Array(keyword.length).fill(fullRows);
      for (let j = 0; j < keyword.length; j++) {

        const keyVal = numKey[j];
        if (keyVal <= remainderChars) {
          colSizes[j]++;
        }
      }
      
      const grid = Array(fullRows + (remainderChars > 0 ? 1 : 0)).fill().map(() => 
        Array(keyword.length).fill(''));
      
      let pos = 0;
      const maxNum = Math.max(...numKey);
      
      for (let num = 1; num <= maxNum; num++) {
        for (let j = 0; j < keyword.length; j++) {
          if (numKey[j] === num) {
            for (let i = 0; i < colSizes[j]; i++) {
              if (pos < text.length) {
                grid[i][j] = text[pos++];
              }
            }
          }
        }
      }
      
      let result = '';
      for (let i = 0; i < grid.length; i++) {
        for (let j = 0; j < keyword.length; j++) {
          if (grid[i][j] !== '') {
            result += grid[i][j];
          }
        }
      }
      
      return result;
    }
  };

  const ngramCipher = (text, n, encrypt) => {
    let result = '';
    let i = 0;
    
    while (i < text.length) {
      for (let j = 0; j < n && i + j < text.length; j++) {
        const char = text[i + j];
        if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
          const isUpper = char >= 'A' && char <= 'Z';
          const base = isUpper ? 65 : 97;
          const charCode = char.charCodeAt(0) - base;
          const shift = 3 + j; 
          
          let resultChar;
          if (encrypt) {
            resultChar = (charCode + shift) % 26;
          } else {
            resultChar = (charCode - shift + 26) % 26;
          }
          
          result += String.fromCharCode(resultChar + base);
        } else {
          result += char;
        }
      }
      i += n;
    }
    
    return result;
  };

  const augustCipher = (text, encrypt) => {
    let result = '';
    
    for (let i = 0; i < text.length; i++) {
      const char = text[i];
      const shift = i + 1; 
      
      if (char >= 'a' && char <= 'z') {
        if (encrypt) {
          result += String.fromCharCode(((char.charCodeAt(0) - 97 + shift) % 26) + 97);
        } else {
          result += String.fromCharCode(((char.charCodeAt(0) - 97 - shift + 26 * 100) % 26) + 97);
        }
      } else if (char >= 'A' && char <= 'Z') {
        if (encrypt) {
          result += String.fromCharCode(((char.charCodeAt(0) - 65 + shift) % 26) + 65);
        } else {
          result += String.fromCharCode(((char.charCodeAt(0) - 65 - shift + 26 * 100) % 26) + 65);
        }
      } else {
        result += char;
      }
    }
    
    return result;
  };

  const renderParams = () => {
    switch(algorithm) {
      case 'caesar':
        return (
          <div className="form-group">
            <label htmlFor="shift">Shift (0-25):</label>
            <input
              type="number"
              id="shift"
              min="0"
              max="25"
              value={params.shift}
              onChange={(e) => handleParamChange('shift', e.target.value)}
            />
          </div>
        );
      case 'affine':
        return (
          <div className="param-container">
            <div className="param-field">
              <label htmlFor="a">Parameter a (coprime with 26):</label>
              <input
                type="number"
                id="a"
                value={params.a}
                onChange={(e) => handleParamChange('a', e.target.value)}
              />
            </div>
            <div className="param-field">
              <label htmlFor="b">Parameter b:</label>
              <input
                type="number"
                id="b"
                value={params.b}
                onChange={(e) => handleParamChange('b', e.target.value)}
              />
            </div>
          </div>
        );
      case 'vigenere':
      case 'beaufort':
      case 'autokey':
        return (
          <div className="form-group">
            <label htmlFor="key">Key:</label>
            <input
              type="text"
              id="key"
              value={params.key}
              onChange={(e) => handleParamChange('key', e.target.value)}
            />
          </div>
        );
      case 'gronsfeld':
        return (
          <div className="form-group">
            <label htmlFor="keyNum">Numeric Key (digits only):</label>
            <input
              type="text"
              id="keyNum"
              value={params.keyNum}
              onChange={(e) => handleParamChange('keyNum', e.target.value.replace(/[^0-9]/g, ''))}
            />
          </div>
        );
      case 'railfence':
        return (
          <div className="form-group">
            <label htmlFor="rails">Number of Rails:</label>
            <input
              type="number"
              id="rails"
              min="2"
              value={params.rails}
              onChange={(e) => handleParamChange('rails', e.target.value)}
            />
          </div>
        );
      case 'route':
      case 'ngram':
        return (
          <div className="form-group">
            <label htmlFor="n">Parameter n:</label>
            <input
              type="number"
              id="n"
              min="2"
              value={params.n}
              onChange={(e) => handleParamChange('n', e.target.value)}
            />
          </div>
        );
      case 'hill':
        return (
          <div className="form-group">
            <label htmlFor="matrix">Matrix (2x2 comma-separated):</label>
            <input
              type="text"
              id="matrix"
              value={params.matrix}
              placeholder="e.g., 2,1,3,4"
              onChange={(e) => handleParamChange('matrix', e.target.value)}
            />
            <div className="matrix-helper">Format: a,b,c,d for matrix [[a,b],[c,d]]</div>
          </div>
        );
      case 'myszkowski':
        return (
          <div className="form-group">
            <label htmlFor="keywordMysz">Keyword:</label>
            <input
              type="text"
              id="keywordMysz"
              value={params.keywordMysz}
              onChange={(e) => handleParamChange('keywordMysz', e.target.value)}
            />
          </div>
        );
      default:
        return null;
    }
  };

  return (
    <div className="crypto-container">
      <div className="header">
        <h1>Cryptography Dashboard</h1>
      </div>
      <div className="dashboard">
        <div className="form-group">
          <label htmlFor="algorithm">Encryption Algorithm:</label>
          <select
            id="algorithm"
            value={algorithm}
            onChange={(e) => setAlgorithm(e.target.value)}
          >
            <option value="caesar">Caesar Cipher</option>
            <option value="affine">Affine Cipher</option>
            <option value="atbash">Atbash Cipher</option>
            <option value="vigenere">Vigenère Cipher</option>
            <option value="beaufort">Beaufort Cipher</option>
            <option value="autokey">Autokey Cipher</option>
            <option value="gronsfeld">Gronsfeld Cipher</option>
            <option value="railfence">Rail Fence Cipher</option>
            <option value="route">Route Cipher</option>
            <option value="hill">Hill Cipher</option>
            <option value="myszkowski">Myszkowski Cipher</option>
            <option value="ngram">N-gram Cipher</option>
            <option value="august">August Cipher</option>
          </select>
          <div className="algo-description">{algorithmDescriptions[algorithm]}</div>
        </div>

        <div className="radio-group">
          <div className="radio-option">
            <input
              type="radio"
              id="encrypt"
              name="operation"
              value="encrypt"
              checked={operation === 'encrypt'}
              onChange={() => setOperation('encrypt')}
            />
            <label htmlFor="encrypt">Encrypt</label>
          </div>
          <div className="radio-option">
            <input
              type="radio"
              id="decrypt"
              name="operation"
              value="decrypt"
              checked={operation === 'decrypt'}
              onChange={() => setOperation('decrypt')}
            />
            <label htmlFor="decrypt">Decrypt</label>
          </div>
        </div>

        {renderParams()}

        <div className="form-group">
          <label htmlFor="inputText">Input Text:</label>
          <textarea
            id="inputText"
            value={inputText}
            onChange={(e) => setInputText(e.target.value)}
            placeholder="Enter text to encrypt or decrypt"
          />
        </div>

        <button className="btn process-btn" onClick={processText}>
          {operation === 'encrypt' ? 'Encrypt' : 'Decrypt'}
        </button>

        {error && <div className="error">{error}</div>}

        {outputText && (
          <div className="result">
            <h3>Result:</h3>
            <div className="output-text">{outputText}</div>
          </div>
        )}
      </div>
    </div>
  );
};

export default CryptoDashboard;