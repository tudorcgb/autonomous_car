#ifndef GPSDATA_H
#define GPSDATA_H

#include <iomanip>
#include <complex>
#include <cmath>
#include <ctime>

/*!
 * \brief The GPSData class, it contains atributes and functions for GPS localization
 */
class GPSData
{
public:
    /*!
     * \name GPSData
     * \brief Constructor method.
     * \param id            car ID
     * \param position      initial car position
     * \param orientation   initial car orientation
     */
    GPSData(const int id = 0,
            const std::complex<double> position = std::complex<double>(0,0),
            const std::complex<double> orientation = std::complex<double>(0,0));

    /*!
     * \name GPSData
     * \brief Copy constructor.
     */
    GPSData(const GPSData &);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   GPSData object to be copied.
     * \return copy of obj
     */
    GPSData& operator=(const GPSData &obj);
//    ~GPSData();

    /*!
     * \name getId
     * \brief Getter method for id.
     * \return id
     */
    int getId();

    /*!
     * \name getPosition
     * \brief Getter method for position.
     * \return position
     */
    std::complex<double> getPosition();

    /*!
     * \name getOrientation
     * \brief Getter method for orientation.
     * \return orientation
     */
    std::complex<double> getOrientation();

    /*!
     * \name getLastUpdateTime
     * \brief Getter method for updateTime.
     * \return updateTime
     */
    std::time_t getLastUpdateTime();

    /*!
     * \name update
     * \brief This method updates informations of the class GPSData.
     * \param position      new position
     * \param orientation   new orientation
     */
    void update(const std::complex<double> position, std::complex<double> orientation);

    /*!
     * \name operator<<
     * \brief OStream redirect operator.
     * \param stream    String on the left
     * \param gpsData   Data to be converted and added to string
     * \return a concatenated ostream
     */
    friend std::ostream& operator<< (std::ostream& stream, const GPSData& gpsData);

private:
    int id;
    std::complex<double> position;
    std::complex<double> orientation;
    std::time_t updateTime;
};

#endif // GPSDATA_H
