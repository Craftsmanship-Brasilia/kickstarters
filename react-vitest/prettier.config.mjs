// Prettier configuration (replaces .prettierrc so it can carry comments).
// `.mjs` = always an ES module, whatever package.json "type" says.
/** @type {import('prettier').Config} */
export default {
  singleQuote: true, // 'text' instead of "text"
  trailingComma: 'all', // trailing commas wherever valid: smaller diffs when adding items
  printWidth: 100, // wrap lines longer than 100 characters (default is 80)
};
