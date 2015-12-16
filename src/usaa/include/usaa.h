/** \file Unity Static Assets Analyzer.
 */

#ifndef USAA_H_
#define USAA_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(USAA_EXPORT)
#   if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN32__)
#       define USAA_EXPORT __declspec(dllexport)
#   else
#       define USAA_EXPORT
#   endif /* !USAA_EXPORT */
#endif

float USAA_EXPORT usaa_example();

#ifdef __cplusplus
}
#endif

#endif /* !USAA_H_ */
