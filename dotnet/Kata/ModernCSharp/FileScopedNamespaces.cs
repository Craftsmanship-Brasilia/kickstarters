// ── File-scoped namespaces (C# 10) ──────────────────────────────────────────
//
// NOW: one line, ends with `;`, applies to the whole file, saves one indentation level.
namespace Kata.ModernCSharp;

// BEFORE (C# 7.3 / netstandard2.0):
//
//   using System;
//   using System.Collections.Generic;
//   using System.Linq;
//
//   namespace Kata.ModernCSharp
//   {
//       public class Example
//       {
//           ...
//       }
//   }
//
// Related: <ImplicitUsings>enable</ImplicitUsings> in the .csproj adds "global usings"
// for System, System.Linq, System.Collections.Generic, System.Threading.Tasks, ...
// so most files no longer need `using` lines at all.
public static class FileScopedNamespaceExample
{
    public static string Namespace => typeof(FileScopedNamespaceExample).Namespace!;
}
