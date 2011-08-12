/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Cisco Systems SIP Stack.
 *
 * The Initial Developer of the Original Code is
 * Cisco Systems (CSCO).
 * Portions created by the Initial Developer are Copyright (C) 2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *  Enda Mannion <emannion@cisco.com>
 *  Suhas Nandakumar <snandaku@cisco.com>
 *  Ethan Hugg <ehugg@cisco.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "cpr_stdio.h"
#include "cpr_string.h"
#ifdef CPR_CSF_LOG
#include "CSFLog.h"
#else
#include <sys/syslog.h>
#endif

/**
 * @def LOG_MAX
 *
 * Constant represents the maximum allowed length for a message
 */
#define LOG_MAX 1024

/**
 * @addtogroup DebugAPIs The CPR Logging Abstractions
 * @ingroup CPR
 * @brief The CPR Debug/Logging APIs
 *
 * @{
 */

/**
 * Debug message sent to syslog(#LOG_DEBUG)
 *
 * @param _format  format string
 * @param ...      variable arg list
 *
 * @return  Return code from vsnprintf
 *
 * @pre (_format not_eq NULL)
 */
int
buginf (const char *_format, ...)
{
    char fmt_buf[LOG_MAX + 1];
    va_list ap;
    int rc;

    va_start(ap, _format);
    rc = vsnprintf(fmt_buf, LOG_MAX, _format, ap);
    va_end(ap);
    if (rc <= 0) {
        return rc;
    }

#ifdef USE_PRINTF
      printf ("%s", fmt_buf);	/* tbd: keep this until syslog is in place */
#elif CPR_CSF_LOG
      CSFLog(CSF_LOG_DEBUG, "", 0, "",fmt_buf);
#else
      syslog(LOG_DEBUG, "%s", fmt_buf); /* temporarily disabled */
#endif
    return rc;
}

/**
 * Debug message sent to syslog(#LOG_DEBUG) that can be larger than #LOG_MAX
 *
 * @param str - a fixed constant string
 *
 * @return  zero(0)
 *
 * @pre (str not_eq NULL)
 */
int
buginf_msg (const char *str)
{
    char buf[LOG_MAX + 1];
    const char *p;
    int16_t len;

    // terminate buffer
    buf[LOG_MAX] = NUL;

    len = (int16_t) strlen(str);

    if (len > LOG_MAX) {
        p = str;
        do {
            memcpy(buf, p, LOG_MAX);
            p += LOG_MAX;
            len -= LOG_MAX;

#ifdef USE_PRINTF
            printf("%s",buf);
#elif CPR_CSF_LOG
            CSFLog(CSF_LOG_DEBUG, "", 0, "",buf);
#else
            syslog(LOG_DEBUG, "%s", buf);
#endif
        } while (len > LOG_MAX);

        if (len) {
#ifdef USE_PRINTF
            printf("%s", (char *)p);
#elif CPR_CSF_LOG
            CSFLog(CSF_LOG_DEBUG, "", 0, "",(char *)p);
#else
            syslog(LOG_DEBUG, "%s", (char *) p);
#endif
            
        }
    } else {
#ifdef USE_PRINTF
        printf("%s", (char *) str);
#elif CPR_CSF_LOG
        CSFLog(CSF_LOG_DEBUG, "", 0, "",str);
#else
        syslog(LOG_DEBUG, "%s", (char *) str);
#endif
    }

    return 0;
}

/**
 * Error message sent to syslog(#LOG_ERR)
 *
 * @param _format  format string
 * @param ...     variable arg list
 *
 * @return  Return code from vsnprintf
 *
 * @pre (_format not_eq NULL)
 */
void
err_msg (const char *_format, ...)
{
    char fmt_buf[LOG_MAX + 1];
    va_list ap;
    int rc;

    va_start(ap, _format);
    rc = vsnprintf(fmt_buf, LOG_MAX, _format, ap);
    va_end(ap);
    if (rc <= 0) {
        return;
    }

#ifdef USE_PRINTF
    printf ("%s", fmt_buf);
#elif CPR_CSF_LOG
    CSFLog(CSF_LOG_ERROR, "", 0, "",fmt_buf);
#else
    syslog((int) LOG_ERR, "%s", fmt_buf);
#endif
}


/**
 * Notice message sent to syslog(#LOG_INFO)
 *
 * @param _format  format string
 * @param ...     variable arg list
 *
 * @return  Return code from vsnprintf
 *
 * @pre (_format not_eq NULL)
 */
void
notice_msg (const char *_format, ...)
{
    char fmt_buf[LOG_MAX + 1];
    va_list ap;
    int rc;

    va_start(ap, _format);
    rc = vsnprintf(fmt_buf, LOG_MAX, _format, ap);
    va_end(ap);
    if (rc <= 0) {
        return;
    }

#ifdef USE_PRINTF
    printf("%s", fmt_buf); /* tbd: remove once syslog in place */
#elif CPR_CSF_LOG
    CSFLog(CSF_LOG_NOTICE, "", 0, "",fmt_buf);
#else
    syslog((int) LOG_NOTICE, "%s", fmt_buf);
#endif
    
}

