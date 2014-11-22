#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include <complex>
#include <string>
#include "Jacobian.h"
#include "Vector3.h"
#include <vector>
class FileManager
{
public:
	/**
	���R�r�A���̃f�[�^�� "fileName" ����ǂݍ���.
	�f�[�^�t�H�[�}�b�g��

	�ŗL�l1 �ŗL�l2 �ŗL�l3
	�ŗL�x�N�g��1.x �ŗL�x�N�g��2.x �ŗL�x�N�g��3.x
	�ŗL�x�N�g��1.y �ŗL�x�N�g��2.y �ŗL�x�N�g��3.y
	�ŗL�x�N�g��1.z �ŗL�x�N�g��2.z �ŗL�x�N�g��3.z

	�̌J��Ԃ�, �ŗL�l,�ŗL�x�N�g���̗v�f�͕��f���^�� ���� �����̏��ɂ��ׂċ󔒂ŋ�؂��Ă���.
	�Ȃ̂�,��̓I�ɂ͈�s�ڂ�

	�ŗL�l1���� �ŗL�l1���� �ŗL�l2���� �ŗL�l2���� �ŗL�l3���� �ŗL�l3����
	
	�ƂȂ��Ă���(�ŗL�x�N�g�������l)
	R�ŏ����o�����f�[�^�Ȃ�, JacobianDataFormatter.rb �ŏ�L�̃t�H�[�}�b�g�ɕϊ��ł���
	�ǂݍ��݌��ʂ�res�Ɋi�[�����
	*/ 
	static void ReadJacobianData(const std::string fileName, std::vector<Jacobian> &res);

	static void ReadCritialPoint(const std::string fileName, std::vector<Vector3d> &res);
private:
	static bool readJacobian(std::ifstream &stream, Jacobian &res);
};

#endif