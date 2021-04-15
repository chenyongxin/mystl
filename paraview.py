import numpy as np
from struct import pack


# String encoder
def encode(string): 
    return str.encode(string)

def vtr(name, x, y, z, **kwargs):
    """
    Write rectilinear grid .vtr file in binary.
    Parameters
    ----------
    name: str
        File name (without '.vtr' extension).
    x, y, z: array-like, float, (N,)
        x, y, z axis 1D grid point.
    **kwargs: dict, optional
        Output fields dictionary object.
        Key: field's name.
        Value: numpy array, 4D. e.g. Value = np.zeros((ndim, nx, ny, nz)) or 
        2D. e.g. Value = np.zeros((ndim, nx*ny*nz))
    """
    nx, ny, nz = x.size, y.size, z.size          # dimensions
    off = 0                                      # offset
    ise, jse, kse = [1, nx], [1, ny], [1, nz]    # start and ending indices

    with open(name+".vtr", 'wb') as fh:
        fh.write(encode( '<VTKFile type="RectilinearGrid" version="0.1" byte_order="LittleEndian">\n'))
        fh.write(encode(f'<RectilinearGrid WholeExtent="{ise[0]} {ise[1]} {jse[0]} {jse[1]} {kse[0]} {kse[1]}">\n'))
        fh.write(encode(f'<Piece Extent="{ise[0]} {ise[1]} {jse[0]} {jse[1]} {kse[0]} {kse[1]}">\n'))
        fh.write(encode( '<Coordinates>\n'))
        fh.write(encode(f'<DataArray type="Float32" Name="x" format="appended" offset="{off}" NumberOfComponents="1"/>\n'))
        off += nx*4 + 4
        fh.write(encode(f'<DataArray type="Float32" Name="y" format="appended" offset="{off}" NumberOfComponents="1"/>\n'))
        off += ny*4 + 4
        fh.write(encode(f'<DataArray type="Float32" Name="z" format="appended" offset="{off}" NumberOfComponents="1"/>\n'))
        off += nz*4 + 4
        fh.write(encode( '</Coordinates>\n'))
        
        # additional info for fields if kwargs is present
        if len(kwargs) > 0:
            fh.write(encode('<PointData>\n'))
            for key, value in kwargs.items():
                ndim = value.shape[0]
                fh.write(encode('<DataArray type="Float32" Name="{}" format="appended" offset="{}" NumberOfComponents="{}"/>\n'
                                 .format(key, off, ndim)))
                off += value.size*4 + 4
            fh.write(encode('</PointData>\n'))

        fh.write(encode('</Piece>\n'))
        fh.write(encode('</RectilinearGrid>\n'))
        fh.write(encode('<AppendedData encoding="raw">\n'))
        fh.write(encode('_'))
        fh.write(pack("i",   4*nx))
        fh.write(pack("f"*nx,  *x))
        fh.write(pack("i",   4*ny))
        fh.write(pack("f"*ny,  *y))
        fh.write(pack("i",   4*nz))
        fh.write(pack("f"*nz,  *z))

        # write fields if present
        if len(kwargs) > 0:
            for value in kwargs.values():
                fh.write(pack("i", 4*value.size))
                fh.write(pack("f"*value.size, *(value.flatten(order='F'))))
        
        fh.write(encode('\n'))
        fh.write(encode('</AppendedData>\n'))
        fh.write(encode('</VTKFile>\n'))