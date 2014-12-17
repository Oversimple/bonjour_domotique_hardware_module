/* 
 * File:   Config.h
 * Author: julien
 *
 * Created on January 14, 2014, 8:43 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    void configTimer(void);
    void configClock(void);
    void configInterruptPriority(void);
    void configPorts(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

