/** \mainpage MD Praktikum 2011
 *
 * \section blatt3_sec Übungsblatt 3
 *
 * Moleküldynamiksimulation
 *
 * \subsection algorithms_subsec Implementierte Verfahren und deren Klassen
 *
 * - LJ_Potential das (skalierte) Lennard-Jones-Potential \f$U(r_{ij})
 * = -4\varepsilon \cdot \left( \frac{\sigma}{r_{ij}} \right)^6 \cdot
 * \left( \left( \frac{\sigma}{r_{ij}} \right)^6 -1 \right)\f$
 * - VelocityVerlet mit \f$O(N^2)\f$ Kraftberechnungen
 *
 * \subsection classes_subsec Weitere modifizierte Klassen
 *
 * - World
 * - Observer
 * - ObserverXYZ
 *
 * \subsection manual_sec Anleitung
 *
 * \code
 * make && ./blatt3
 * \endcode
 *
 */
#ifndef _DEFINES_HPP
#define _DEFINES_HPP

// define the dimension of the particles
#define DIM 3
// reals in double precision
typedef double real;
// square define
#define sqr(_x) ((_x)*(_x)) 
// define BorderType
enum BorderType {unknown = 0, leaving = 1, periodic = 2};

#endif // _DEFINES_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin cino=g0,t0:
