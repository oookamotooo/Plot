#include "Geodesic.h"
#include "Field.h"
#include <vector>
using namespace std;

namespace{
	void AccuracyLong(){

	}
	Vector3d SearchNextBandPoint(Field *field, const vector<Vector3d> &bandPoints, const int &focusedPointId, const Vector3d &prevPoint,
		const Vector3d &leafNormal, const double &Delta){

		const double Step = 0.5;
		const double Len = 0.1;
		const double SqrDelta = Delta * Delta;

		const Vector3d start = bandPoints[focusedPointId];
		const Vector3d Vec01 = start - prevPoint;

		//�����_�ŗ��z�Ƃ���ʒu�ɍł��߂��_��,���̎��̗��z�l�Ƃ̍�
		Vector3d nearestPoint;
		double nearestSqrDist = 10.0;

		for (int i = 0; i < bandPoints.size(); i++){

			//���߂ɒT�����߂� �ΏۂƂ���_�̋߂�����T���悤�ɂ���.
			int nowId = (focusedPointId - 2 + i + bandPoints.size()) % bandPoints.size();

			//�ׂ̓_
			int nextId = (nowId + 1) % bandPoints.size();

			const double DeltaSplit = 1.0 / 100.0; //������
			for (double s = 0; s < 1.0; s += DeltaSplit){

				Vector3d br = bandPoints[nowId] * (1.0 - s) + bandPoints[nextId] * s;

				const int NumRunge = 300;	//�����Q�N�b�^�̌J��Ԃ���
				for (int n = 0; n < NumRunge; n++){
					br += field->rungeKutta(br, Step).normalized()*Len;

					Vector3d dir = br - start;

					// �����ɂ���(�L��������ɂȂ�)���͖̂���
					if (Vec01.dot(dir) < 0)
						continue;

					// �@���ƒ��p�ɋ߂� => ���ʏ�ɂ��邩�`�F�b�N
					// TODO : dir, leafNormal�𐳋K�����Ă��Ȃ�.
					if (std::abs(dir.dot(leafNormal)) >= 0.1)
						continue;

					double sqrDist = (br - start).squaredNorm();

					//��ԗ��z�l�ɋ߂��_�͕ۑ�����.
					if (std::abs(sqrDist - SqrDelta) < nearestSqrDist){
						nearestSqrDist = std::abs(sqrDist - SqrDelta);
						nearestPoint = br;
					}

					//���z�l����������,�␳�������ĕԂ�.
					if (std::abs(sqrDist - SqrDelta) < 0.05){
						double epsilon = (nearestPoint - start).squaredNorm();
						return start + (nearestPoint - start) * std::sqrt(SqrDelta / epsilon);
					}
				}
			}
		}

		return start;
	}
}

Geodesic::Geodesic(Field *field, const Vector3d &criticalPoint, const std::vector<Eigen::Vector3d> &roundPoints)
{
	Calc(field, criticalPoint, roundPoints);
}

void Geodesic::Calc(Field *field, const Eigen::Vector3d &criticalPoint, const std::vector<Eigen::Vector3d> &roundPoints)
{

	bool existData = true;

	double arc = 0;	//�ʂ̒���
	double L = 50;

	vector<Vector3d> Band[3];
	int nowId = 0;	//���݂�Id
	while (arc < L){

		for (int r = 0; r < Band[nowId].size(); r++){
			int next = (r + 1) % Band[nowId].size();
			int prev = (r - 1 + Band[nowId].size()) % Band[nowId].size();
			Vector3d leaf = (Band[nowId][next] - Band[nowId][prev]).normalized();

			int nextId = (nowId + 1) % 3;
			int prevId = (nowId + 2) % 3;

			Band[nextId][r] = SearchNextBandPoint(field, Band[nowId], r, Band[prevId][r], leaf, 0.1);
		}


	}
}
