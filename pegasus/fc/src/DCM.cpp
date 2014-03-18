/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 *
 *  Direction Cosine Matrix
 *  by William Premerlani and Paul Bizard
 *  (cf: http://gentlenav.googlecode.com/files/DCMDraft2.pdf)
 */

#include "fc/include/DCM.h"
#include "core/include/Trace.h"

namespace pegasus{
    namespace fc {

        using namespace pegasus::core::math;

        DCM::DCM() :
        _mKp(0.2f),
        _mKi(0.0009f),//0.0087f
        _mOmegaP(0.0f, 0.0f, 0.0f),
        _mOmegaI(0.0f, 0.0f, 0.0f),
        _mDCM() {

            _mDCM.setup();

        }

        void DCM::update(Attitude_t* att, float G_dt)
        {
            _mAtt = att;
            matrixUpdate(G_dt);

            normalize();

            driftCorrection();

            _mDCM.toEULER(&att->euler.roll, &att->euler.pitch, &att->euler.yaw);

        }

        void DCM::matrixUpdate(float G_dt)
        {
            Vector3f gyro;
            gyro.x = _mAtt->gyro.x;
            gyro.y = _mAtt->gyro.y;
            gyro.z = _mAtt->gyro.z;

            _mOmega = (gyro + _mOmegaI + _mOmegaP); // Add P I term

            _mDCM.multiply(_mOmega * G_dt);
        }

        void DCM::normalize()
        {
            float error;
            float renorm;
            Vector3f tmp0,tmp1,tmp2;

            error = _mDCM.a * _mDCM.b;                          // eq.18 (X * Y)
            tmp0 = _mDCM.a - (_mDCM.b * (0.5f * error));        // eq.19 (X - ((error/2) * Y)
            tmp1 = _mDCM.b - (_mDCM.a * (0.5f * error));        // eq.19 (Y - ((error/2) * X)
            tmp2 = tmp0 % tmp1;                                 // eq.20 Z = X * Y (Cross product)

            // Renorm eq.21
            renorm = 0.5f * (3 - (tmp0 * tmp0));
            _mDCM.a = tmp0 * renorm;

            renorm = 0.5f * (3 - (tmp1 * tmp1));
            _mDCM.b = tmp1 * renorm;

            renorm = 0.5f * (3 - (tmp2 * tmp2));
            _mDCM.c = tmp2 * renorm;
        }

        void DCM::driftCorrection()
        {

            Matrix3f tmpDCM = _mDCM;
            Vector3f error;
            Vector3f tmpScaledI;
            Vector3f acc;

            acc.x = _mAtt->acc.x; // in m/s²
            acc.y = _mAtt->acc.y; // in m/s²
            acc.z = _mAtt->acc.z; // in m/s²
            acc.normalize();

            error = acc % tmpDCM.c; // Cross Product
            _mOmegaP = error * _mKp;
            tmpScaledI = error * _mKi;
            _mOmegaI += tmpScaledI;
        }

    } /* namespace fc */
} /* namespace pegasus */
